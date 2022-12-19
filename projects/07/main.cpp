#include <iostream>
#include <string>
#include <filesystem>

#include "bulk_reader.h"
#include "stream_logger.h"
#include "file_logger.h"

using namespace std;

int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "No bulk size passed" << endl;
        return 1;
    }

    const unsigned int bulkSize = stoi(argv[1]);
    BulkReader bulkReader(bulkSize);

    StreamLogger consoleLogger(cout);
    bulkReader.addFlushObserver(&consoleLogger);

    FileLogger fileLogger(filesystem::current_path());
    bulkReader.addFlushObserver(&fileLogger);

    for (string line; getline(cin, line);) {
        bulkReader.addCmd(line);
    }

    bulkReader.eof();

    return 0;
}
