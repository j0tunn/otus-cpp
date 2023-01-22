#include <boost/uuid/detail/md5.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/crc.hpp>
#include "hash.h"

using namespace std;

namespace hash {

string crc32(const string& str) {
    boost::crc_32_type result;
    result.process_bytes(str.data(), str.length());

    return to_string(result.checksum());
}

string md5(const string& str) {
    using boostMd5 = boost::uuids::detail::md5;

    boostMd5 hash;
    boostMd5::digest_type digest;

    hash.process_bytes(str.data(), str.size());
    hash.get_digest(digest);

    const int* intDigest = reinterpret_cast<const int*>(&digest);
    string result;
    boost::algorithm::hex(intDigest, intDigest + (sizeof(boostMd5::digest_type)/sizeof(int)), back_inserter(result));

    return str;
}

}
