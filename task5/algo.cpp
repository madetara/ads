//
// Created by dv.kab on 3/18/2020.
//

#include "algo.h"

#include <functional>
#include <random>

bst<int> get_random_tree(size_t n, const std::function<size_t(void)> &get_random) {
    auto result = bst<int>();

    while (n--) {
        result.push(get_random());
    }

    return result;
}

double aggregate_tree(size_t n, std::function<double(bst<int>)> agg) {
    auto repetitions = RANDOM_TREE_COUNT;
    auto result = 0.0;

    std::default_random_engine rand(time(nullptr));
    std::uniform_int_distribution<int> dist(1, 10000);

    while (repetitions--) {
        auto tree = get_random_tree(n, [&]() -> size_t { return dist(rand); });
        result += agg(tree);
    }

    return result;
}

double count_mean_height(size_t n) {
    return aggregate_tree(n, [](const bst<int> &tree) -> double { return tree.height(); }) / RANDOM_TREE_COUNT;
}

double count_avl_chances(size_t n) {
    return aggregate_tree(n, [](const bst<int> &tree) -> double { return tree.is_avl(); }) / RANDOM_TREE_COUNT;
}
