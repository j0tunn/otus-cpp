#include <sstream>
#include <algorithm>
#include <iterator>
#include <locale>
#include <memory>
#include "utils.h"

using namespace std;

struct CommaIsSpace : ctype<char> {
    CommaIsSpace() : ctype<char>(get_table()) {}

    static mask const* get_table() {
        static mask rc[table_size];

        rc[','] = ctype_base::space;
        rc[' '] = ctype_base::space;
        rc['\n'] = ctype_base::space;

        return &rc[0];
    }
};

pair<unsigned, vector<float> > parseTestData(const string& src) {
    istringstream lineStream(src);
    lineStream.imbue(locale(lineStream.getloc(), new CommaIsSpace));

    unsigned expected;
    lineStream >> expected;

    vector<float> features;
    copy(istream_iterator<float>(lineStream), istream_iterator<float>(), back_inserter(features));

    return make_pair(move(expected), move(features));
}

unsigned predict(ModelCalcerWrapper& calcer, const vector<float>& features) {
    auto res = calcer.CalcMulti(features);
    return distance(res.begin(), max_element(res.begin(), res.end()));
}

