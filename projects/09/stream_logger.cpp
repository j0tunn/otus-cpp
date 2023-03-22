#include "stream_logger.h"

using namespace std;

StreamLogger::StreamLogger(ostream& out)
    : out_(out)
{}

void StreamLogger::onFlush(const vector<Command>& bulk) {
    out_ << "bulk: ";

    for (unsigned int i = 0; i < bulk.size(); ++i) {
        out_ << bulk[i].val;
        if (i < bulk.size() - 1) {
            out_ << ", ";
        }
    }

    out_ << endl;
}
