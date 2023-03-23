#pragma once

#include <vector>
#include "command.h"

class IFlushObserver {
public:
    virtual ~IFlushObserver() = default;
    virtual void onFlush(const std::vector<Command>& bulk) = 0;
};

class FlushObservable {
public:
    void addFlushObserver(IFlushObserver* observer);

protected:
    void notifyFlush_(const std::vector<Command>& bulk);

private:
    std::vector<IFlushObserver*> observers_;
};
