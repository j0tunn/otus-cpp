#include <iostream>
#include <string>
#include <utility>
#include <cstdlib>
#include <filesystem>
#include <algorithm>
#include "map-reduce.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: mapreduce <src> <mnum> <rnum>" << endl;
        return 1;
    }

    try {
        const string src = string(argv[1]);
        const unsigned int mappersCount = atoi(argv[2]);
        const unsigned int reducesCount = atoi(argv[3]);

        filesystem::path inputFile(src);

        for (unsigned int prefixLen = 1;; ++prefixLen) {
            const vector<string> res = MapReduce<pair<bool, string>>(mappersCount, reducesCount)
                .setMapper([prefixLen](const string& line) { return line.substr(0, prefixLen); })
                .setReducer([](const string& val, const pair<bool, string>& acc) { return make_pair(get<0>(acc) || (get<1>(acc) == val), val); })
                .setReduceAccSerializer([](const pair<bool, string>& res) { return get<0>(res) ? "1" : "0"; })
                .run(inputFile, make_pair(false, ""));

            if (res.end() == find(res.begin(), res.end(), "1")) {
                cout << prefixLen << endl;
                break;
            }
        }
    } catch(const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }

    return 0;
}
