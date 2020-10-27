#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>

template<typename Arg, typename... Args>
void print_args(const Arg& arg, const Args&... args) {
    if constexpr (sizeof...(args) > 0) {
        std::cout << arg << ", ";
        print_args(args...);
    }
    else
        std::cout << arg << std::endl;
}

template<typename FwdIt>
void print(FwdIt first, FwdIt last) {
    while (first != last && std::next(first) != last)
        std::cout << *first++ << " ";

    if (first != last)
        std::cout << *first << std::endl;
}

inline void print_array(int arr[], int len) { // Ausgabe eines int[]
    for (int i = 0; i < len; ++i)
        std::cout << arr[i] << ' ';
    std::cout << std::endl;
}

int random_int(int min_val, int max_val);

#endif // UTILITY_H
