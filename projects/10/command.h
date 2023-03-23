#pragma once

#include <string>
#include <ctime>

struct Command {
    explicit Command(const std::string& cmd)
        : val(cmd)
        , timestamp(std::time(0))
    {}

    Command(const Command&) = default;

    std::string val;
    std::time_t timestamp;
};
