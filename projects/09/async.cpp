#include "async.h"
#include "bulk_reader.h"
#include "stream_logger.h"
#include "file_logger.h"
#include "thread_logger.h"
#include <filesystem>
#include <string>

using namespace std;

namespace async {

class Handle {
public:
    Handle(size_t bulkSize)
        : reader_(bulkSize)
        , consoleLogger_(1, []() { return new StreamLogger(cout); })
        , fileLogger_(2, []() { return new FileLogger(filesystem::current_path()); })
    {
        reader_.addFlushObserver(&consoleLogger_);
        reader_.addFlushObserver(&fileLogger_);
    }

    ~Handle() {
        reader_.eof();
    }

    void addCmd(const string& cmd) {
        reader_.addCmd(cmd);
    }

private:
    BulkReader reader_;
    ThreadLogger consoleLogger_;
    ThreadLogger fileLogger_;
};

Handle* connect(size_t bulkSize) {
    return new Handle(bulkSize);
}

void receive(Handle* h, const char *data, size_t size) {
    h->addCmd(string(data, size));
}

void disconnect(Handle* h) {
    delete h;
}

}
