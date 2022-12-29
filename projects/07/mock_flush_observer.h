#pragma once

#include <gmock/gmock.h>

#include "flush_observer.h"

class MockFlushObserver : public IFlushObserver {
public:
    MOCK_METHOD(void, onFlush, (const std::vector<Command>&), (override));
};
