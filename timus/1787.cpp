#include <iostream>

int main()
{
    unsigned short k, n, a;
    short res = 0;

    std::cin >> k >> n;

    for (; n > 0; n--) {
        std::cin >> a;
        res += a - k;
        if (res < 0) res = 0;
    }

    std::cout << res << std::endl;

    return 0;
}

