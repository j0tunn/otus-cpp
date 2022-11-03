#pragma once

#include <string>
#include <numeric>
#include <type_traits>
#include <tuple>

template <typename T, std::size_t N>
struct integerIP {
    static std::string toString(T ip) {
        return integerIP<T, N/2>::toString(ip >> (N/2) * 8) + "." + integerIP<T, N/2>::toString(ip);
    }
};

template <typename T>
struct integerIP<T, 1> {
    static std::string toString(T ip) {
        return std::to_string(ip & 0xFF);
    }
};

// integers: use numeric_limits to check if type is integer
template <typename T, typename Fake = typename std::enable_if<std::numeric_limits<T>::is_integer, void>::type>
std::string ipToString(T ip) {
    return integerIP<T, sizeof(T)>::toString(ip);
}

// string: check by c_str() method
template <typename T>
std::string ipToString(const T& ip, decltype(ip.c_str(), 100500) noop = 100500) {
    return ip;
}

template <typename T>
std::string ipToString(T begin, T end) {
    if (begin == end) {
        return "";
    }

    const std::string piece = std::to_string(*begin++);

    return begin == end
        ? piece
        : piece + "." + ipToString(begin, end);
}

// list & vector: check by emplace_back() method
template <typename T, typename Fake = decltype(T().emplace_back())>
std::string ipToString(const T& ip) {
    return ipToString(ip.begin(), ip.end());
}

template <typename T, size_t N>
struct tupleIP {
    static std::string toString(const T& ip) {
        static_assert(
            std::is_same<typename std::tuple_element<N - 1, T>::type, typename std::tuple_element<N - 2, T>::type>::value,
            "Tuple should have same types for all elements."
        );

        return tupleIP<T, N - 1>::toString(ip) + "." + std::to_string(std::get<N - 1>(ip));
    }
};

template <typename T>
struct tupleIP<T, 1> {
    static std::string toString(const T& ip) {
        return std::to_string(std::get<0>(ip));
    }
};

template <typename T>
struct tupleIP<T, 0> {
    static std::string toString(const T& ip) {
        return "";
    }
};

// tuple
template <typename T, size_t N = std::tuple_size<T>()>
std::string ipToString(const T& ip) {
    return tupleIP<T, N>::toString(ip);
}
