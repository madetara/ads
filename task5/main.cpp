//
// Created by dv.kab on 3/18/2020.
//

#include <iostream>

#include "algo.h"

int main() {
    std::cout << "Mean height for 100-element tree: " << count_mean_height(100) << std::endl;

    std::cout << "Mean height for 1000-element tree: " << count_mean_height(1000) << std::endl;

    std::cout << "Mean height for 10000-element tree: " << count_mean_height(10000) << std::endl;


    std::cout << "Chance to be an AVL-tree for 100-element tree: " << count_avl_chances(100) * 100 << "%" << std::endl;

    std::cout << "Chance to be an AVL-tree for 1000-element tree: " << count_avl_chances(1000) * 100 << "%" << std::endl;

    std::cout << "Chance to be an AVL-tree for 10000-element tree: " << count_avl_chances(10000) * 100 << "%" << std::endl;
}
