#include "bulk_allocator.h"
#include "list.h"

#include <iostream>
#include <map>

using namespace std;

int factorial(const int i) {
    return i == 0 ? 1 : i * factorial(i - 1);
}

int main(int, char **) {
    auto m1 = map<int, int>{};
    for (int i = 0; i < 10; ++i) {
        m1[i] = factorial(i);
    }

    auto m2 = map<int, int, less<int>, BulkAllocator<pair<const int, int>, 10>>{};
    for (int i = 0; i < 10; ++i) {
        m2[i] = factorial(i);
    }

    for (const auto& [key, value] : m2) {
        cout << key << " " << value << endl;
    }

    List<int> l1;
    for (int i = 0; i < 10; ++i) {
        l1.push_back(i);
    }

    List<int, BulkAllocator<int, 10> > l2;
    for (int i = 0; i < 10; ++i) {
        l2.push_back(i);
    }

    for (auto it = l2.begin(); it != l2.end(); ++it) {
        cout << *it << endl;
    }

    return 0;
}
