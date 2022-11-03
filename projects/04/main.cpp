#include <iostream>
#include "ip_to_string.h"

using namespace std;
#include <vector>
#include <list>

int main(int, char **) {
    cout << ipToString(int8_t{-1}) << endl;
    cout << ipToString(int16_t{0}) << endl;
    cout << ipToString(int32_t{2130706433}) << endl;
    cout << ipToString(int64_t{8875824491850138409}) << endl;
    cout << ipToString(string{"Hello, World!"}) << endl;
    cout << ipToString(vector<int>{100, 200, 300, 400}) << endl;
    cout << ipToString(list<short>{400, 300, 200, 100}) << endl;
    cout << ipToString(make_tuple(123, 456, 789, 0)) << endl;

    return 0;
}
