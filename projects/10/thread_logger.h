#pragma once

#include <functional>
#include <thread>
#include <vector>
#include <queue>
#include <condition_variable>
#include <mutex>
#include "flush_observer.h"

class ThreadLogger : public IFlushObserver {
public:
    using SubLoggerCreateFn = std::function<IFlushObserver*()>;

    ThreadLogger(std::size_t maxThreads, SubLoggerCreateFn createSubLogger);
    ~ThreadLogger();

    void onFlush(const std::vector<Command>& bulk) override;

private:
    void workerThread();
    void stop();

    std::size_t maxThreads_;
    SubLoggerCreateFn createSubLogger_;
    std::vector<std::thread> threads_;
    std::queue<std::vector<Command> > bulks_;
	std::mutex dataAccess_;
    std::condition_variable dataChanged_;
    bool stopped_;
};
