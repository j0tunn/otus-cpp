#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <filesystem>
#include <boost/program_options/errors.hpp>
#include "cli.h"

using namespace std;
using namespace testing;
namespace po = boost::program_options;

TEST(Cli_parse, onHelp_ShouldNotExecuteParseCallback) {
    vector<const char*> args = {"./cmd", "--help"};

    EXPECT_NO_THROW({
        Cli()
            .onParse([](const Opts&) {
                throw logic_error("Should not be called");
            })
            .parse(args.size(), args.data());
    });
}

TEST(Cli_parse, onHelp_ShouldExecuteHelpCallback) {
    vector<const char*> args = {"./cmd", "--help"};
    string helpMsg = "";

    Cli()
        .onHelp([&helpMsg](const string& msg) {
            helpMsg = msg;
        })
        .parse(args.size(), args.data());

    EXPECT_NE(helpMsg, "");
}

TEST(Cli_parse, onParse_ShouldNotExecuteHelpCallback) {
    vector<const char*> args = {"./cmd"};

    EXPECT_NO_THROW({
        Cli()
            .onHelp([](const string&) {
                throw logic_error("Should not be called");
            })
            .parse(args.size(), args.data());
    });
}

TEST(Cli_parse, onParse_ShouldExecuteParseCallback) {
    vector<const char*> args = {"./cmd"};
    string msg = "";

    Cli()
        .onParse([&msg](const Opts&) {
            msg = "Successfully called";
        })
        .parse(args.size(), args.data());

    EXPECT_NE(msg, "");
}

TEST(Cli_parse, dirs_ShouldBeCurrentDirByDefault) {
    vector<const char*> args{"./cmd"};

    Cli()
        .onParse([](const Opts& opts) {
            EXPECT_EQ(opts.dirs.size(), 1);
            EXPECT_EQ(opts.dirs[0], filesystem::current_path().string());
        })
        .parse(args.size(), args.data());
}

TEST(Cli_parse, dirs_ShouldAcceptFewValues) {
    vector<const char*> args{"./cmd", "foo", "bar"};

    Cli()
        .onParse([](const Opts& opts) {
            EXPECT_EQ(opts.dirs, vector<filesystem::path>({"foo", "bar"}));
        })
        .parse(args.size(), args.data());
}

TEST(Cli_parse, excludeDirs_ShouldBeEmptyByDefault) {
    vector<const char*> args{"./cmd"};

    Cli()
        .onParse([](const Opts& opts) {
            EXPECT_EQ(opts.excludeDirs, vector<string>());
        })
        .parse(args.size(), args.data());
}

TEST(Cli_parse, excludeDirs_ShouldAcceptFewValues) {
    vector<const char*> args{"./cmd", "--exclude", "foo", "--exclude", "bar"};

    Cli()
        .onParse([](const Opts& opts) {
            EXPECT_EQ(opts.excludeDirs, vector<string>({"foo", "bar"}));
        })
        .parse(args.size(), args.data());
}

TEST(Cli_parse, files_ShouldBeEmptyByDefault) {
    vector<const char*> args{"./cmd"};

    Cli()
        .onParse([](const Opts& opts) {
            EXPECT_EQ(opts.files, vector<string>());
        })
        .parse(args.size(), args.data());
}

TEST(Cli_parse, files_ShouldAcceptFewValues) {
    vector<const char*> args{"./cmd", "--file", "foo", "--file", "bar"};

    Cli()
        .onParse([](const Opts& opts) {
            EXPECT_EQ(opts.files, vector<string>({"foo", "bar"}));
        })
        .parse(args.size(), args.data());
}

TEST(Cli_parse, scanLevel_ShouldBeZeroByDefault) {
    vector<const char*> args{"./cmd"};

    Cli()
        .onParse([](const Opts& opts) {
            EXPECT_EQ(opts.scanLevel, 0);
        })
        .parse(args.size(), args.data());
}

TEST(Cli_parse, scanLevel_ShouldBePassedNumber) {
    vector<const char*> args{"./cmd", "--scan-level", "1"};

    Cli()
        .onParse([](const Opts& opts) {
            EXPECT_EQ(opts.scanLevel, 1);
        })
        .parse(args.size(), args.data());
}

TEST(Cli_parse, minSize_ShouldBeOneByteByDefault) {
    vector<const char*> args{"./cmd"};

    Cli()
        .onParse([](const Opts& opts) {
            EXPECT_EQ(opts.minSize, 1);
        })
        .parse(args.size(), args.data());
}

TEST(Cli_parse, minSize_ShouldBePassedNumber) {
    vector<const char*> args{"./cmd", "--min-size", "100500"};

    Cli()
        .onParse([](const Opts& opts) {
            EXPECT_EQ(opts.minSize, 100500);
        })
        .parse(args.size(), args.data());
}

TEST(Cli_parse, chunkSize_ShouldBe64BytesByDefault) {
    vector<const char*> args{"./cmd"};

    Cli()
        .onParse([](const Opts& opts) {
            EXPECT_EQ(opts.chunkSize, 64);
        })
        .parse(args.size(), args.data());
}

TEST(Cli_parse, chunkSize_ShouldBePassedNumber) {
    vector<const char*> args{"./cmd", "--chunk-size", "100500"};

    Cli()
        .onParse([](const Opts& opts) {
            EXPECT_EQ(opts.chunkSize, 100500);
        })
        .parse(args.size(), args.data());
}

TEST(Cli_parse, hash_ShouldBeMd5ByDefault) {
    vector<const char*> args{"./cmd"};

    Cli()
        .onParse([](const Opts& opts) {
            EXPECT_EQ(opts.hashType, HashType::md5);
        })
        .parse(args.size(), args.data());
}

TEST(Cli_parse, hash_ShouldBeParsedFromCmdLine) {
    vector<const char*> args{"./cmd", "--hash", "crc32"};

    Cli()
        .onParse([](const Opts& opts) {
            EXPECT_EQ(opts.hashType, HashType::crc32);
        })
        .parse(args.size(), args.data());
}

TEST(Cli_parse, hash_ShouldThrowOnUnknownHashType) {
    vector<const char*> args{"./cmd", "--hash", "foo"};

    EXPECT_THROW({
        try
        {
            Cli().parse(args.size(), args.data());
        }
        catch(const po::invalid_option_value& e)
        {
            EXPECT_EQ("--hash", e.get_option_name());
            throw;
        }
    }, po::invalid_option_value);
}
