#include <sstream>
#include <fstream>
#include <thread>
#include "file_logger.h"
#include "stream_logger.h"

using namespace std;

FileLogger::FileLogger(const filesystem::path& dir)
    : logDir_(dir)
{}

void FileLogger::onFlush(const vector<Command>& bulk) {
    if (bulk.size() == 0) {
        return;
    }

    stringstream fileName;
    fileName << "bulk" << bulk[0].timestamp << "_" << this_thread::get_id() << ".log";

    ofstream out(logDir_ / fileName.str());
    StreamLogger subLogger(out);
    subLogger.onFlush(bulk);
}
