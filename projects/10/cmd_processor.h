#pragma once

#include "bulk_router.h"
#include "thread_logger.h"
#include <string>

class CmdProcessor {
public:
    CmdProcessor(unsigned int bulkSize);
    ~CmdProcessor();

    void handleCmd(const std::string& cmd, unsigned long sessionId);

private:
    BulkRouter bulkRouter_;
    ThreadLogger consoleLogger_;
    ThreadLogger fileLogger_;
};
