#include <sstream>
#include "file_logger.h"
#include "stream_logger.h"
#include "fs.h"

using namespace std;

FileLogger::FileLogger(const filesystem::path& dir)
    : logDir_(dir)
{}

void FileLogger::onFlush(const vector<Command>& bulk) {
    if (bulk.size() == 0) {
        return;
    }

    const string fileName = "bulk" + to_string(bulk[0].timestamp) + ".log";

    stringstream out;
    StreamLogger subLogger(out);
    subLogger.onFlush(bulk);

    writeFile(logDir_ / fileName, out.str());
}
