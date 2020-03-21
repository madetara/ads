//
// Created by dv.kab on 3/18/2020.
//

#ifndef TASK5_ALGO_H
#define TASK5_ALGO_H

#include "bst.h"
#include <functional>

#define RANDOM_TREE_COUNT 1000

bst<int> get_random_tree(size_t n, const std::function<size_t(void)> &get_random);

double count_mean_height(size_t n);

double count_avl_chances(size_t n);

#endif //TASK5_ALGO_H
