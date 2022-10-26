#include <iostream>

int main()
{
    unsigned short ax, by, b, a;

    std::cin >> ax >> by >> b /* ignore */ >> b >> a;
    std::cout << ax - a << " " << by - b << std::endl;

    return 0;
}

