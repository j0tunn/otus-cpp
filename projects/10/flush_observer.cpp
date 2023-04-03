#include "flush_observer.h"

using namespace std;

void FlushObservable::addFlushObserver(IFlushObserver* observer) {
    observers_.push_back(observer);
}

void FlushObservable::notifyFlush_(const vector<Command>& bulk) {
    for (auto observer : observers_) {
        observer->onFlush(bulk);
    }
}
