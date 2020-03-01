#include <iostream>
#include <cassert>
#include <list>
#include <fstream>

#include "LinkedList.h"
#include "elem.h"
#include "utils.h"

void run_tests();
void print_line();
void compare(size_t count);

int main() {
    run_tests();
    return 0;
}

void run_tests() {
    {
        auto list = new LinkedList<int>{10, -10, 20};
        assert(list->count_positives() == 2);
        delete list;
        print_line();
    }
    {
        auto list = new LinkedList<int>{1, 2, 1, 1, 2, 3, 1};
        size_t count = 0;
        auto max = list->find_max_and_count(count);
        assert(max == 3);
        assert(count == 1);
        delete list;
        print_line();
    }
    {
        auto list = new LinkedList<int>{1, 2, 1, 1, 2, 1};
        size_t count = 0;
        auto max = list->find_max_and_count(count);
        assert(max == 2);
        assert(count == 2);
        delete list;
        print_line();
    }
    {
        auto positives = new LinkedList<int>();
        auto negatives = new LinkedList<int>();
        auto base = new LinkedList<int>{1, -1, 2, 5, -42, 666};
        std::cout << "Input list: " << *base << std::endl
                  << "Splitting..." << std::endl;
        base->split_by_signum(negatives, positives);
        std::cout << "Negatives: " << *negatives << std::endl
                  << "Positives: " << *positives << std::endl
                  << "Input list: " << *base << std::endl;
        delete base;
        delete positives;
        delete negatives;
        print_line();
    }
    {
        auto list = new LinkedList<int>{1, 2, 3, 4, 5};
        std::cout << "Input list: " << *list << std::endl
                  << "Reversing..." << std::endl;
        list->reverse();
        std::cout << "Reversed: " << *list << std::endl;
        delete list;
        print_line();
    }
    {
        auto list = new LinkedList<int>{1};
        std::cout << "Input list: " << *list << std::endl
                  << "Reversing..." << std::endl;
        list->reverse();

        std::cout << "Reversed: " << *list << std::endl;
        delete list;
        print_line();
    }
    {
        std::cout << "Testing iterators" << std::endl;
        LinkedList<int> list1, list2;
        list1.push_back(10);
        std::cout << (++ --list1.end() == list1.end()) << std::endl;
        std::cout << (list2.end() != list1.end()) << std::endl;
        std::cout << (list1.end() == list1.end()) << std::endl;
        std::cout << *(--list1.end()) << std::endl;
        print_line();
    }
    {
        auto list = new LinkedList<int>{5, 1, 4, -7, 3, 12};
        for (auto x: *list) {
            std::cout << x << " ";
        }
        std::cout << std::endl;
    }
    {
        auto a = elem(1), b = elem(2);
        elem::cmp_count = 0;

        if (a < b) {}

        assert(elem::cmp_count == 1);

        a = b;
        assert(elem::assign_count == 1);
    }

    std::cout << "Tests passed!" << std::endl;

    const size_t million = 100 * 100 * 100;
    for (auto multiplier : std::list<size_t>{1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024})
        compare(multiplier * million);
}

std::list<elem>* get_std(size_t count) {
    auto result = new std::list<elem>();

    for (size_t i = 0; i < count; ++i) {
        result->push_back(elem(42));
    }

    return result;
}

LinkedList<elem>* get_my(size_t count) {
    auto result = new LinkedList<elem>();

    for (size_t i = 0; i < count; ++i) {
        result->push_back(elem(42));
    }

    return result;
}

void compare(size_t count) {
    volatile auto my_list = get_my(count);
    auto my_time = measure([&my_list] { my_list->reverse(); });
    delete my_list;

    volatile auto std_list = get_std(count);
    auto std_time = measure([&std_list] { std_list->reverse(); });
    delete std_list;

    std::ofstream result;
    result.open("result.csv", std::ios_base::app);
    std::cout.precision(20);
    std::cout << "Elems: " << count << std::endl
              << "My time: " << my_time << std::endl
              << "STD time: " << std_time << std::endl;
    result << count << "," << my_time << "," << std_time << std::endl;
    result.close();


    print_line();
}

void print_line() {
    std::cout << "-----------------------------------------------------" << std::endl;
}
