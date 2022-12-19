#pragma once

#include <iostream>
#include "flush_observer.h"

class StreamLogger : public IFlushObserver {
public:
    explicit StreamLogger(std::ostream& out);
    void onFlush(const std::vector<Command>& bulk) override;

private:
    std::ostream& out_;
};
