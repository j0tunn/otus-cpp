#pragma once

#include <string>
#include <functional>
#include <memory>
#include <vector>
#include "command.h"

class ReaderState;
using StateSwitchFn = std::function<void(std::unique_ptr<ReaderState>&&)>;

///
class ReaderState {
public:
    ReaderState(StateSwitchFn switchState, unsigned int bulkSize);
    virtual ~ReaderState() = default;

    const std::vector<Command>& getBulk() const;

    virtual void addCmd(const std::string& cmd);

    virtual void startBulk() = 0;
    virtual void finishBulk() = 0;
    virtual void eof() = 0;
    virtual bool isAutoMode() const = 0;

protected:
    template <typename T>
    void setNewState();

    std::vector<Command> bulk_;
    StateSwitchFn switchState_;
    unsigned int bulkSize_;
};

///
class AutoModeState : public ReaderState {
public:
    AutoModeState(StateSwitchFn switchState, unsigned int bulkSize);
    ~AutoModeState() = default;

    void addCmd(const std::string& cmd) override;
    void startBulk() override;
    void finishBulk() override;
    void eof() override;
    bool isAutoMode() const override;
};

///
class ManualModeState : public ReaderState {
public:
    ManualModeState(StateSwitchFn switchState, unsigned int bulkSize);

    void startBulk() override;
    void finishBulk() override;
    void eof() override;
    bool isAutoMode() const override;

private:
    unsigned int startCounter_;
};
