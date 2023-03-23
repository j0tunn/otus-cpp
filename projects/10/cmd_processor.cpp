#include "cmd_processor.h"
#include "stream_logger.h"
#include "file_logger.h"
#include <iostream>

using namespace std;

CmdProcessor::CmdProcessor(unsigned int bulkSize)
        : bulkRouter_(bulkSize)
        , consoleLogger_(1, []() { return new StreamLogger(cout); })
        , fileLogger_(2, []() { return new FileLogger(filesystem::current_path()); })
{
    bulkRouter_.addFlushObserver(&consoleLogger_);
    bulkRouter_.addFlushObserver(&fileLogger_);
}

CmdProcessor::~CmdProcessor() {
    bulkRouter_.eof();
}

void CmdProcessor::handleCmd(const string& cmd, unsigned long sessionId) {
    bulkRouter_.addCmd(cmd, sessionId);
}
