#include <string>
#include <iostream>

#include "async.h"

using namespace std;

int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "No bulk size passed" << endl;
        return 1;
    }

    const unsigned int bulkSize = stoi(argv[1]);
    async::Handle* h = async::connect(bulkSize);

    for (string line; getline(cin, line);) {
        async::receive(h, &line[0], line.size());
        if (line == "exit") {
            break;
        }
    }

    async::disconnect(h);

    return 0;
}
