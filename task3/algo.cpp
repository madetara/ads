//
// Created by dv.kab on 3/1/2020.
//

#include <vector>

#include "ordered_set.h"

std::vector<int> eratosthenes_set(int n) {
    if (n < 2)
        throw std::logic_error("n should be greater than 1");

    auto result = std::vector<int>();
    auto non_primes = ordered_set<int>();

    for (int i = 2; i < n; ++i) {
        if (non_primes.contains(i))
            continue;

        result.push_back(i);

        for (int j = i; j < n; j += i)
            non_primes.add(j);
    }

    return result;
}

std::vector<int> eratosthenes_array(int n) {
    if (n < 2)
        throw std::logic_error("n should be greater than 1");

    auto result = std::vector<int>();
    auto non_primes = new bool[n + 1];

    for (int i = 2; i < n; ++i) {
        if (non_primes[i])
            continue;

        result.push_back(i);

        for (int j = i; j < n; ++j) {
            non_primes[j] = false;
        }
    }


    delete[] non_primes;
    return result;
}
