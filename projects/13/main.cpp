#include <iostream>
#include <string>
#include <fstream>
#include <catboost/wrapped_calcer.h>
#include "utils.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: fashion_mnist <test-file> <model-file>\n";
        return 1;
    }

    const string testFile(argv[1]);
    const string modelFile(argv[2]);

    ModelCalcerWrapper calcer(modelFile);
    ifstream testData(testFile);

    unsigned total = 0;
    unsigned correct = 0;
    for (string line; getline(testData, line);) {
        const auto [expected, features] = parseTestData(line);
        const auto actual = predict(calcer, features);

        ++total;
        if (actual == expected) {
            ++correct;
        }
    }

    cout << static_cast<double>(correct)/total << endl;

    return 0;
}
