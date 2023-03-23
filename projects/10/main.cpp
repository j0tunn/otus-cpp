#include "cmd_processor.h"
#include <boost/asio.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <iostream>

using boost::asio::ip::tcp;
using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;

using namespace std;

void processCmd(const string& cmd) {
    cout << cmd << endl;
}

awaitable<void> handleSession(tcp::socket socket, unsigned long sessionId, CmdProcessor& cmdProcessor) {
    string data;
    try {
        for (;;) {
            size_t n = co_await boost::asio::async_read_until(socket, boost::asio::dynamic_buffer(data), boost::regex("(\r?\n)+"), use_awaitable);

            cmdProcessor.handleCmd(data.substr(0, n - 1), sessionId);
            data.erase(0, n);
        }
    } catch (boost::system::system_error& e) {
        if (e.code() == boost::asio::error::eof) {
            if (data.size() > 0) {
                cmdProcessor.handleCmd(data, sessionId);
            }
        } else {
            throw e;
        }
    } catch (exception& e) {
        cerr << "session handle error: " << e.what() << "\n";
    }
}

awaitable<void> listen(unsigned short port, CmdProcessor& cmdProcessor) {
    auto executor = co_await boost::asio::this_coro::executor;
    tcp::acceptor acceptor(executor, {tcp::v4(), port});

    for (unsigned long sessionId = 0;; ++sessionId) {
        tcp::socket socket = co_await acceptor.async_accept(use_awaitable);
        co_spawn(executor, handleSession(move(socket), sessionId, cmdProcessor), detached);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: bulk_server <port> <bulk_size>\n";
        return 1;
    }

    try {
        const unsigned short port = atoi(argv[1]);
        const unsigned int bulkSize = atoi(argv[2]);
        CmdProcessor cmdProcessor(bulkSize);

        boost::asio::io_context io_context(1);

        boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
        signals.async_wait([&](auto, auto){ io_context.stop(); });

        co_spawn(io_context, listen(port, cmdProcessor), detached);

        io_context.run();
    } catch (exception& e) {
        cerr << "Exception: " << e.what() << "\n";

        return 1;
    }

    return 0;
}
