#pragma once

#include <vector>
#include "command.h"

class IFlushObserver {
public:
    virtual ~IFlushObserver() = default;
    virtual void onFlush(const std::vector<Command>& bulk) = 0;
};
