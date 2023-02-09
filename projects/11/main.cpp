#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/write.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "db.h"

using boost::asio::ip::tcp;
using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;
namespace this_coro = boost::asio::this_coro;

using namespace std;

void processInsert(const string& cmd, DataBase& db) {
    vector<string> pieces;
    boost::split(pieces, cmd, boost::is_space());

    if (pieces.size() < 4) {
        throw logic_error(string("Bad request: ") + cmd);
    }

    db.insert(pieces[1], pieces[2], pieces[3]);
}

void processTruncate(const string& cmd, DataBase& db) {
    vector<string> pieces;
    boost::split(pieces, cmd, boost::is_space());

    if (pieces.size() < 2) {
        throw logic_error(string("Bad request: ") + cmd);
    }

    db.truncate(pieces[1]);
}

string processCmd(const string& cmd, DataBase& db) {
    cout << cmd << endl;

    if (boost::starts_with(cmd, "INSERT")) {
        processInsert(cmd, db);
        return "";
    }

    if (boost::starts_with(cmd, "TRUNCATE")) {
        processTruncate(cmd, db);
        return "";
    }

    if (cmd == "INTERSECTION") {
        return db.getIntersection();
    }

    if (cmd == "SYMMETRIC_DIFFERENCE") {
        return db.getSymmetricDifference();
    }

    throw logic_error(string("Unknown command: ") + cmd);
}

awaitable<string> getRequest(tcp::socket& socket) {
    char data[256] = {0};
    size_t n = co_await socket.async_read_some(boost::asio::buffer(data), use_awaitable);

    string res(data, n);
    if (n && data[n - 1] != '\n') {
        res += co_await getRequest(socket);
    }

    boost::trim(res);

    co_return res;
}

awaitable<void> handleRequest(tcp::socket& socket, DataBase& db) {
    const string req = co_await getRequest(socket);

    vector<string> commands;
    boost::split_regex(commands, req, boost::regex("(\r?\n)+"));

    for (const auto& cmd : commands) {
        string resp;

        try {
            resp += processCmd(cmd, db) + "OK";
        } catch (exception& e) {
            resp = string("ERR: ") + e.what();
        }

        co_await async_write(socket, boost::asio::buffer(resp + "\n"), use_awaitable);
    }
}

awaitable<void> handleSession(tcp::socket socket) {
    try {
        DataBase db;

        for (;;) {
            co_await handleRequest(socket, db);
        }
    } catch (boost::system::system_error& e) {
        if (e.code() != boost::asio::error::eof) {
            throw e;
        }
    } catch (exception& e) {
        cerr << "session handle error: " << e.what() << "\n";
    }
}

awaitable<void> listen(unsigned short port) {
    auto executor = co_await this_coro::executor;
    tcp::acceptor acceptor(executor, {tcp::v4(), port});

    for (;;) {
        tcp::socket socket = co_await acceptor.async_accept(use_awaitable);
        co_spawn(executor, handleSession(move(socket)), detached);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: join_server <port>\n";
        return 1;
    }

    try {
        boost::asio::io_context io_context(1);

        boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
        signals.async_wait([&](auto, auto){ io_context.stop(); });

        co_spawn(io_context, listen(atoi(argv[1])), detached);

        io_context.run();
    } catch (exception& e) {
        cerr << "Exception: " << e.what() << "\n";

        return 1;
    }

    return 0;
}
