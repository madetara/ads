//
// Created by dv.kab on 3/1/2020.
//

#ifndef TASK3_ORDERED_SET_H
#define TASK3_ORDERED_SET_H

#include <functional>

#include "bst.h"

template <typename T, typename Cmp = std::less<T>>
class ordered_set {
    bst<T, Cmp> *_storage;

    void _swap(const ordered_set<T, Cmp> &other) {
        std::swap(_storage, other._storage);
    }

public:
    ordered_set(): _storage(new bst<T, Cmp>()) {}

    ordered_set(const ordered_set<T, Cmp> &other) : _storage(bst(other._storage)) {}

    ordered_set(ordered_set<T, Cmp> &&other) {
        this->_swap(other);
    }

    ordered_set<T, Cmp> &operator=(const ordered_set<T, Cmp> &other) {
        if (this != &other)
            this->_swap(other);

        return *this;
    }

    ~ordered_set() {
        delete _storage;
    }

    void add(const T &value) {
        if (contains(value))
            return;
        _storage->push(value);
    }

    bool contains(const T &value) const {
        return _storage->find(value);
    }

    void remove(const T &value) {
        _storage->remove(value);
    }
};

#endif //TASK3_ORDERED_SET_H
