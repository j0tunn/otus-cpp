#include <sstream>
#include <filesystem>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>
#include "cli.h"

namespace po = boost::program_options;
using namespace std;

Cli::Cli()
    : helpCb_([](const string&) {})
    , parseCb_([](const Opts&) {})
{}

Cli& Cli::onHelp(HelpCallback cb) {
    helpCb_ = cb;

    return *this;
}

Cli& Cli::onParse(ParseCallback cb) {
    parseCb_ = cb;

    return *this;
}

void validate(boost::any& val,
              const std::vector<std::string>& values,
              HashType*, int)
{
    po::validators::check_first_occurrence(val);
    const string& strVal = po::validators::get_single_string(values);

    if (strVal == "md5") {
        val = boost::any(HashType::md5);
    } else if (strVal == "crc32") {
        val = boost::any(HashType::crc32);
    } else {
        throw po::invalid_option_value(strVal);
    }
}


void Cli::parse(int argc, const char* const* argv) {
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("dir", po::value<vector<filesystem::path> >()->default_value({filesystem::current_path()}, "[cwd]"), "include dirs")
        ("exclude", po::value<vector<string> >(), "directory names to exclude")
        ("file", po::value<vector<string> >(), "file masks allowed to compare (case insensitive)")
        ("scan-level", po::value<unsigned int>()->default_value(0), "scan level")
        ("min-size", po::value<unsigned long>()->default_value(1), "min file size (in bytes)")
        ("chunk-size,S", po::value<unsigned long>()->default_value(64), "chunk size to read per iteration")
        ("hash,H", po::value<HashType>()->default_value(HashType::md5, "md5"), "hash type (md5 | crc32)")
    ;

    po::positional_options_description p;
    p.add("dir", -1);

    po::variables_map vm;
    po::store(
        po::command_line_parser(argc, argv)
            .options(desc)
            .positional(p)
            .run(),
        vm
    );

    if (vm.count("help")) {
        stringstream s;
        s << "Usage: bayan [options] [dir1 dir2 ...]" << endl << desc;

        helpCb_(s.str());

        return;
    }

    po::notify(vm);

    parseCb_(Opts{
        .dirs = vm["dir"].as<vector<filesystem::path> >(),
        .excludeDirs = vm.count("exclude") ? vm["exclude"].as<vector<string> >() : vector<string>(),
        .files = vm.count("file") ? vm["file"].as<vector<string> >() : vector<string>(),
        .scanLevel = vm["scan-level"].as<unsigned int>(),
        .minSize = vm["min-size"].as<unsigned long>(),
        .chunkSize = vm["chunk-size"].as<unsigned long>(),
        .hashType = vm["hash"].as<HashType>()
    });
}
