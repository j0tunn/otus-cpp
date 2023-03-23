#pragma once

#include <string>
#include <filesystem>
#include "flush_observer.h"

class FileLogger : public IFlushObserver {
public:
    FileLogger(const std::filesystem::path& dir);
    void onFlush(const std::vector<Command>& bulk) override;

private:
    const std::filesystem::path logDir_;
};
