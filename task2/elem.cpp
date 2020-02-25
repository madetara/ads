//
// Created by dv.kab on 2/12/2020.
//

#include "elem.h"

size_t elem::assign_count;
size_t elem::cmp_count;

std::ostream &operator<<(std::ostream &os, const elem &v) {
    os << v.data;
    return os;
}

std::istream &operator>>(std::istream &is, elem &v) {
    int x;
    is >> x;
    v = elem(x);
    return is;
}

elem &elem::operator=(elem other) {
    this->swap(other);
    elem::assign_count++;
    return *this;
}

void elem::swap(elem other) {
    std::swap(this->data, other.data);
}

bool elem::operator==(const elem &other) const {
    elem::cmp_count++;
    return this->data == other.data;
}

bool elem::operator<(const elem &other) const {
    elem::cmp_count++;
    return this->data < other.data;
}
