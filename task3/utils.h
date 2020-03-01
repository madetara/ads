//
// Created by dv.kab on 3/1/2020.
//

#ifndef TASK3_UTILS_H
#define TASK3_UTILS_H

#include <ctime>
#include <functional>

inline double measure(const std::function<void(void)>& f) {
    auto start = clock();
    f();
    return double(clock() - start) / CLOCKS_PER_SEC;
}

#endif //TASK3_UTILS_H
