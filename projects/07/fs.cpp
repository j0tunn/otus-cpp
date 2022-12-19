#include "fs.h"

#include <fstream>

using namespace std;

void writeFile(const filesystem::path& path, const string& content) {
    ofstream out(path);
    out << content;
}
