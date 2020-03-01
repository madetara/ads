#include <iostream>
#include <cassert>

#include "bst.h"
#include "ordered_set.h"
#include "algo.h"
#include "utils.h"

void run_bst_tests();
void run_set_tests();
void eratosthenes_compare();

int main() {
    run_bst_tests();
    std::cout << "BST tests passed!" << std::endl;

    run_set_tests();
    std::cout << "Set tests passed!" << std::endl;

    eratosthenes_compare();

    return 0;
}

void run_bst_tests() {
    {
        auto tree = bst<int>();
        tree.push(42);
        assert(tree.find(42));
    }
    {
        auto tree = bst<double>();
        tree.push(42);
        assert(!(tree.find(666)));
    }
    {
        auto tree = bst<int>();
        tree.push(1);
        tree.push(2);
        tree.push(3);
        assert(tree.find(2));
        assert(!(tree.find(4)));
    }
    {
        auto tree = bst<int>();
        for (auto x : {7, 5, 1, 4, 234, -10, 52})
            tree.push(x);

        assert(tree.max() == 234);
        assert(tree.min() == -10);
    }
    {
        auto tree = bst<int>();
        for (auto x : {5, 4, 1, 8, 3, 10, -42, 666, 7, 6})
            tree.push(x);

        assert(tree.next_greater_or_same(5) == 6);
        assert(tree.next_greater_or_same(4) == 5);
        assert(tree.next_greater_or_same(1) == 3);
        assert(tree.next_greater_or_same(8) == 10);
        assert(tree.next_greater_or_same(10) == 666);
        assert(tree.next_greater_or_same(-42) == 1);
        assert(tree.next_greater_or_same(6) == 7);
        assert(tree.next_greater_or_same(7) == 8);
        assert(tree.next_greater_or_same(3) == 4);
        assert(tree.next_greater_or_same(666) == 666);
    }
    {
        auto tree = bst<int>();
        for (auto x : {1, 10, 1, 1, 1, 1})
            tree.push(x);

        assert(tree.next_greater_or_same(1) == 1);
    }
    {
        auto tree = bst<int>();
        for (auto x : {5, 4, 1, 8, 3, 10, -42, 666, 7, 6})
            tree.push(x);

        assert(tree.next_less_or_same(5) == 4);
        assert(tree.next_less_or_same(4) == 3);
        assert(tree.next_less_or_same(1) == -42);
        assert(tree.next_less_or_same(8) == 7);
        assert(tree.next_less_or_same(3) == 1);
        assert(tree.next_less_or_same(10) == 8);
        assert(tree.next_less_or_same(-42) == -42);
        assert(tree.next_less_or_same(666) == 10);
        assert(tree.next_less_or_same(7) == 6);
        assert(tree.next_less_or_same(6) == 5);
    }
    {
        auto tree = bst<int>();
        for (auto x : {1, 7, 2, 5, 9, 15, 4, 26})
            tree.push(x);

        tree.infix_print(std::cout);
    }
    {
        auto tree = bst<int>();
        for (auto x : {5, 3, 5, 7, 10, 6, 1, 2, 3, 9})
            tree.push(x);

        std::cout << "Before removal: " << std::endl;
        tree.infix_print(std::cout);

        assert(tree.find(9));

        tree.remove(9);
        assert(!tree.find(9));

        std::cout << "After removal: " << std::endl;
        tree.infix_print(std::cout);
    }
    {
        auto tree = bst<int>();
        for (auto x : {1, 2, 3})
            tree.push(x);

        tree.remove(1);

        tree.infix_print(std::cout);
    }
    {
        auto tree = bst<int>();
        auto another_tree = bst<int>();

        for (auto x : {5, 1, 3, 10, 42, -5, 7, 15}) {
            tree.push(x);
            another_tree.push(x);
        }

        assert(tree.equals(another_tree));
    }
}

void run_set_tests() {
    {
        auto set = ordered_set<int>();
        set.add(42);
        assert(set.contains(42));
        assert(!set.contains(666));
    }
    {
        auto set = ordered_set<int>();
        set.add(42);
        assert(set.contains(42));

        set.remove(42);
        assert(!set.contains(42));
    }
}

void eratosthenes_compare() {
    for (auto n : {1000, 10000, 15000, 20000, 30000, 50000, 100000, 250000, 500000, 1000000}) {
        {
            auto set_time = measure([&n]() { volatile auto result = eratosthenes_set(n); });
            std::cout << "Set-based: " << "N: " << n << " Time: " << set_time << std::endl;
        }
        {
            auto array_time = measure([&n]() { volatile auto result = eratosthenes_array(n); });
            std::cout << "Array-based: " << "N: " << n << " Time: " << array_time << std::endl;
        }
    }
}
