//
// Created by dv.kab on 2/19/2020.
//

#ifndef ASD2_LINKEDLIST_H
#define ASD2_LINKEDLIST_H

#include "Node.h"
#include <cstddef>

template <typename T>
struct LinkedList {
    class iterator {
        Node<T>* current;
    public:
        explicit iterator(Node<T> *current): current(current) {}

        iterator &operator++() {
            if (current != nullptr)
                current = current->next;
            return *this;
        }

        iterator operator++(int) {
            auto result = *this;
            ++(*this);
            return result;
        }

        iterator &operator--() {
            if (current != nullptr)
                current = current->prev;
            return *this;
        }

        iterator operator--(int) {
            auto result = *this;
            --(this);
            return result;
        }

        bool operator==(iterator other) {
            return this->current == other.current;
        }

        bool operator!=(iterator other) {
            return !(*this == other);
        }

        T operator*() {
            return this->current->value;
        }

        T* operator->() {
            return &(this->current->value);
        }

        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::bidirectional_iterator_tag;
    };


    LinkedList() {
        _head = _end = new EndNode<T>();
    }
    LinkedList(std::initializer_list<T> list) {
        this->_head = this->_end = new EndNode<T>();
        for (auto x: list)
            this->push_back(x);
    }
    LinkedList(T* data, size_t n) {
        this->_head = this->_end = new EndNode<T>();
        for (size_t i = 0; i < n; ++i)
            this->push_back(data[i]);
    }
    LinkedList(const LinkedList<T> &other) {
        this->swap(other);
    }

    LinkedList<T> &operator=(const LinkedList<T> &other) {
        if (this != &other)
            this->swap(other);

        return *this;
    }

    void push_back(T value) {
        auto next = new Node<T>(value, nullptr, _end);
        push_back(next);
    }

    void push_back(Node<T> *node) {
        if (_head->isEnd()) {
            node->next = _end;
            node->prev = _end;
            _end->prev = node;
            _head = node;
            return;
        }

        auto prev = _end->prev;
        prev->next = node;
        node->prev = prev;
        node->next = _end;
        _end->prev = node;
    }

    size_t count_positives() const {
        auto result = 0;
        auto current = _head;

        while (!current->isEnd()) {
            if (current->value > 0)
                ++result;

            current = current->next;
        }

        return result;
    }

    T find_max_and_count(size_t &count) const {
        count = 0;

        if (_head->isEnd())
            throw std::invalid_argument("list is empty");

        auto max = _head->value;
        auto current = _head;

        while (!current->isEnd()) {
            if (current->value == max)
                ++count;

            if (current->value > max) {
                max = current->value;
                count = 1;
            }

            current = current->next;
        }

        return max;
    }

    void split_by_signum(LinkedList<T> *negatives, LinkedList<T> *positives) {
        if (_head->isEnd()) {
            return;
        }

        auto current = _head;
        while (!current->isEnd()) {
            auto next = current->next;
            if (current->value > 0)
                positives->push_back(current);
            else
                negatives->push_back(current);

            current = next;
        }

        this->_head = this->_end;
    }

    void reverse() {
        auto current = _head;

        while (!current->isEnd()) {
            auto next = current->next;
            std::swap(current->prev, current->next);
            current = next;
        }

        std::swap(_head, _end->prev);
    }

    iterator begin() {
        return iterator(this->_head);
    }

    iterator end() {
        return iterator(this->_end);
    }

    ~LinkedList() {
        auto current = _head;

        while (current != nullptr) {
            auto next = current->next;
            delete current;
            current = next;
        }
    }

    friend std::ostream &operator<<(std::ostream &os, const LinkedList<T> &l) {
        if (l._head->isEnd())
            os << "Empty!";
        auto current = l._head;

        while (!current->isEnd()) {
            os << current->value << " ";
            current = current->next;
        }

        return os;
    }

private:
    Node<T> *_head;
    Node<T> *_end;

    void swap(LinkedList<T> &other) {
        std::swap(this->_head, other._head);
        std::swap(this->_end, other, _end);
    }
};

#endif //ASD2_LINKEDLIST_H
