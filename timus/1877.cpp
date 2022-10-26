#include <iostream>

int main()
{
    unsigned short lastDigit1, lastDigit2;

    std::cin.ignore(3);
    std::cin >> lastDigit1;
    std::cin.ignore(3);
    std::cin >> lastDigit2;

    if (!(lastDigit1 & 1) || (lastDigit2 & 1)) {
        std::cout << "yes" << std::endl;
    } else {
        std::cout << "no" << std::endl;
    }

    return 0;
}
