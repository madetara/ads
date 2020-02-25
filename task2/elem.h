//
// Created by dv.kab on 2/12/2020.
//

#ifndef ASD1_ELEM_H
#define ASD1_ELEM_H


#include <cstddef>
#include <iostream>

class elem {
    int data;

    void swap(elem other);

public:
    static size_t cmp_count;
    static size_t assign_count;

    elem(): data(0) {}
    explicit elem(int x): data(x) {}

    elem(const elem &other) {
        data = other.data;
    }

    elem(elem &&other) noexcept {
        this->swap(other);
    }

    friend std::ostream &operator<<(std::ostream &os, const elem &v);
    friend std::istream &operator>>(std::istream &is, elem &v);
    elem& operator=(elem other);
    bool operator==(const elem &other) const;
    bool operator<(const elem &other) const;
};


#endif //ASD1_ELEM_H
