#include <iostream>
#include "bulk_reader.h"

using namespace std;

BulkReader::BulkReader(unsigned int bulkSize)
    : state_(new AutoModeState([this](unique_ptr<ReaderState>&& newState) { this->setNewState(move(newState)); }, bulkSize))
{}

void BulkReader::addCmd(const string& cmd) {
    if (cmd == "{") {
        state_->startBulk();
        return;
    }

    if (cmd == "}") {
        state_->finishBulk();
        return;
    }

    state_->addCmd(cmd);
}

void BulkReader::eof() {
    state_->eof();
}

void BulkReader::setNewState(unique_ptr<ReaderState>&& newState) {
    notifyFlush_(state_->getBulk());
    state_ = move(newState);
}

void BulkReader::addFlushObserver(IFlushObserver* observer) {
    observers_.push_back(observer);
}

void BulkReader::notifyFlush_(const vector<Command>& bulk) {
    for (auto observer : observers_) {
        observer->onFlush(bulk);
    }
}
