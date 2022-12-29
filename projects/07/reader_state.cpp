#include "reader_state.h"

using namespace std;

ReaderState::ReaderState(StateSwitchFn switchState, unsigned int bulkSize)
    : bulk_()
    , switchState_(switchState)
    , bulkSize_(bulkSize)
{}

template <typename T>
void ReaderState::setNewState() {
    switchState_(unique_ptr<ReaderState>(new T(switchState_, bulkSize_)));
}

const vector<Command>& ReaderState::getBulk() const {
    return bulk_;
};

void ReaderState::addCmd(const std::string& cmd) {
    bulk_.emplace_back(Command{cmd});
}

/// AutoModeState
AutoModeState::AutoModeState(StateSwitchFn switchState, unsigned int bulkSize)
    : ReaderState(switchState, bulkSize)
{}

void AutoModeState::addCmd(const std::string& cmd) {
    ReaderState::addCmd(cmd);

    if (bulk_.size() >= bulkSize_) {
        setNewState<AutoModeState>();
    }
}

void AutoModeState::startBulk() {
    setNewState<ManualModeState>();
}

void AutoModeState::finishBulk() {
}

void AutoModeState::eof() {
    setNewState<AutoModeState>();
}

/// ManualModeState
ManualModeState::ManualModeState(StateSwitchFn switchState, unsigned int bulkSize)
    : ReaderState(switchState, bulkSize)
    , startCounter_(1)
{}

void ManualModeState::startBulk() {
    ++startCounter_;
}

void ManualModeState::finishBulk() {
    if (--startCounter_ == 0) {
        setNewState<AutoModeState>();
    }
}

void ManualModeState::eof() {
}
