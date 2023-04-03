#include "thread_logger.h"
#include <memory>

using namespace std;

ThreadLogger::ThreadLogger(size_t maxThreads, ThreadLogger::SubLoggerCreateFn createSubLogger)
    : maxThreads_(maxThreads)
    , createSubLogger_(createSubLogger)
    , stopped_(false)
{
    threads_.reserve(maxThreads);
}

ThreadLogger::~ThreadLogger() {
    stop();

    for (auto& t : threads_) {
        t.join();
    }
}

void ThreadLogger::onFlush(const vector<Command>& bulk) {
    if (threads_.size() < maxThreads_) {
        threads_.emplace_back(&ThreadLogger::workerThread, this);
    }

    {
        lock_guard<mutex> guard{dataAccess_};
        bulks_.push(bulk);
    }

    dataChanged_.notify_one();
};

void ThreadLogger::workerThread() {
    unique_ptr<IFlushObserver> pSubLogger{createSubLogger_()};
    vector<Command> nextBulk;

    for (;;) {
        {
            unique_lock<mutex> lck{dataAccess_};
            dataChanged_.wait(lck, [this]() {
                return stopped_ || !bulks_.empty();
            });

            if (stopped_) {
                break;
            }

            nextBulk = bulks_.front();
            bulks_.pop();
        }

        pSubLogger->onFlush(nextBulk);
    }
}

void ThreadLogger::stop() {
    {
        lock_guard<mutex> guard{dataAccess_};
        stopped_ = true;
    }

    dataChanged_.notify_all();
}
