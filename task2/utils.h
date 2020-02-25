//
// Created by dv.kab on 2/23/2020.
//

#ifndef ASD2_UTILS_H
#define ASD2_UTILS_H

#include <ctime>
#include <functional>

inline double measure(const std::function<void(void)>& f) {
    auto start = clock();
    f();
    return double(clock() - start) / CLOCKS_PER_SEC;
}

#endif //ASD2_UTILS_H
