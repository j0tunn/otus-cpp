#include <iostream>
#include "matrix.h"

using namespace std;

int main(int, char **) {
    Matrix<unsigned short, 2, 0> m;

    for (unsigned short i = 0; i <= 9; ++i) {
        m[i][i] = i;
        m[i][9 - i] = 9 - i;
    }

    for (unsigned short i = 1; i <= 8; ++i) {
        for (unsigned short j = 1; j <= 8; ++j) {
            cout << m[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    cout << "size: " << m.size() << endl << endl;

    for (const auto& element : m) {
        unsigned short x = element.first[0];
        unsigned short y = element.first[1];

        cout << x << "," << y << ": " << element.second << endl;
    }

    return 0;
}
