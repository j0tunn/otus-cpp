#pragma once

#include <string>
#include <functional>
#include "opts.h"

using HelpCallback = std::function<void(const std::string&)>;
using ParseCallback = std::function<void(const Opts&)>;

class Cli {
public:
    Cli();
    ~Cli() = default;

    Cli& onHelp(HelpCallback);
    Cli& onParse(ParseCallback);

    void parse(int argc, const char* const* argv);

private:
    HelpCallback helpCb_;
    ParseCallback parseCb_;
};
