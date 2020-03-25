//
// Created by dv.kab on 3/25/2020.
//

#ifndef TASK6_P_QUEUE_H
#define TASK6_P_QUEUE_H


#include <vector>
#include <functional>

template<
        typename T,
        typename Container = std::vector<T>,
        typename Compare = std::less<typename Container::value_type>>
class p_queue {
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
    explicit p_queue(Container container = Container(), Compare compare = Compare()) : _data(container),
                                                                                       _cmp(compare) {}

    p_queue(const p_queue<T, Container, Compare> &other) : p_queue() {
        std::copy(other._data.begin(), other._data.end(), _data.begin());
    }

    p_queue(p_queue<T, Container, Compare> &&other) {
        this->_swap(other);
    }

    template<typename It>
    p_queue(It begin, It end) : p_queue() {
        auto current = begin;
        while (current != end) {
            push(*current);
            ++current;
        }
    }

    p_queue(const std::initializer_list<T> &ll) : p_queue(ll.begin(), ll.end()) {
    }

    p_queue(const T *arr, size_type size) : p_queue(arr, arr + size) {
    }

    p_queue<T, Container, Compare> &operator=(const p_queue<T, Container, Compare> &other) {
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

        return _data[0];
    }

    void push(const T &value) {
        _data.push_back(value);
        _bubble_up(_data.size() - 1);
    }

    void pop() {
        if (empty())
            throw std::logic_error("queue is empty");

        std::swap(_data[0], _data[_data.size() - 1]);
        _data.pop_back();
        _bubble_down();
    }

    void merge(p_queue<T, Container, Compare> &other) {
        while (!other.empty()) {
            push(other.top());
            other.pop();
        }
    }

private:
    void _swap(p_queue<T, Container, Compare> &other) {
        std::swap(_data, other._data);
    }

    inline void _bubble_up(size_type idx) {
        while (idx > 0 && _cmp(_data[_parent(idx)], _data[idx])) {
            std::swap(_data[_parent(idx)], _data[idx]);
            idx = _parent(idx);
        }
    }

    inline void _bubble_down() {
        auto idx = 0;
        while (idx < _data.size()) {
            auto biggest = idx;
            auto left = _left(idx);
            auto right = _right(idx);

            if (left < _data.size() && _cmp(_data[idx], _data[left])) {
                biggest = left;
            }

            if (right < _data.size() && _cmp(_data[biggest], _data[right])) {
                biggest = right;
            }

            if (idx == biggest)
                break;

            std::swap(_data[idx], _data[biggest]);
            idx = biggest;
        }
    }

    inline static size_type _parent(size_type idx) {
        return (idx - 1) / 2;
    }

    inline static size_type _left(size_type idx) {
        return 2 * idx + 1;
    }

    inline static size_type _right(size_type idx) {
        return 2 * idx + 2;
    }
};

#endif //TASK6_P_QUEUE_H
