//
// Created by dv.kab on 3/25/2020.
//

#ifndef TASK6_P_QUEUE_ARR_H
#define TASK6_P_QUEUE_ARR_H

#include <vector>
#include <functional>

template<
        typename T,
        typename Container = std::vector<T>,
        typename Compare = std::less<typename Container::value_type>>
class p_queue_arr {
public:
    using value_type      = typename Container::value_type;
    using reference       = typename Container::reference;
    using const_reference = typename Container::const_reference;
    using size_type       = typename Container::size_type;
    using container_type  = Container;
    using value_compare   = Compare;

private:
    Container _data;
    Compare _cmp;

public:
    explicit p_queue_arr(Container container = Container(), Compare compare = Compare()) : _data(container),
                                                                                           _cmp(compare) {}

    p_queue_arr(const p_queue_arr<T, Container, Compare> &other) : p_queue_arr() {
        std::copy(other._data.begin(), other._data.end(), _data.begin());
    }

    p_queue_arr(p_queue_arr<T, Container, Compare> &&other) {
        this->_swap(other);
    }

    template<typename It>
    p_queue_arr(It begin, It end) : p_queue_arr() {
        auto current = begin;
        while (current != end) {
            push(*current);
            ++current;
        }
    }

    p_queue_arr(const std::initializer_list<T> &ll) : p_queue_arr(ll.begin(), ll.end()) {
    }

    p_queue_arr(const T *arr, size_type size) : p_queue_arr(arr, arr + size) {
    }

    p_queue_arr<T, Container, Compare> &operator=(const p_queue_arr<T, Container, Compare> &other) {
        if (this != &other) {
            auto temp(other);
            this->_swap(temp);
        }

        return *this;
    }

    bool empty() const {
        return _data.empty();
    }

    size_type size() const {
        return _data.size();
    }

    const_reference top() const {
        if (empty())
            throw std::logic_error("queue is empty");

        return _data[_data.size() - 1];
    }

    void push(const T &value) {
        _data.push_back(value);
        _heapify();
    }

    void pop() {
        if (empty())
            throw std::logic_error("queue is empty");

        _data.pop_back();
    }

    void merge(p_queue<T, Container, Compare> &other) {
        while (!other.empty()) {
            push(other.top());
            other.pop();
        }
    }

private:
    void _swap(p_queue_arr<T, Container, Compare> &other) {
        std::swap(_data, other._data);
    }

    inline void _heapify() {
        auto idx = _data.size() - 1;
        while (idx > 0 && _cmp(_data[idx], _data[idx - 1])) {
            std::swap(_data[idx], _data[idx - 1]);
            --idx;
        }
    }
};

#endif //TASK6_P_QUEUE_ARR_H
