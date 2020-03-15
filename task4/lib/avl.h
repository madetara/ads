//
// Created by dv.kab on 3/12/2020.
//

#ifndef TASK4_AVL_H
#define TASK4_AVL_H

#include <queue>
#include <functional>

template <typename T, typename Cmp = std::less<T>, typename Alloc = std::allocator<T>>
class avl {
    struct node {
        T _data;
        node *_parent;
        node *_left;
        node *_right;
    };

    node *_root;
    node *_fake;
    Cmp _cmp;
    Alloc _al;

public:
    avl() : _cmp(Cmp()), _al(Alloc()) {
        _root = _fake = reinterpret_cast<node *>(new char[sizeof(node)]);
    }

    void push(const T &value) {
        _push(value);
    }

    bool find(const T &value) const {
        _find(value);
    }

    ~avl() {
        auto to_delete = std::queue<node *>();
        to_delete.push(_root);

        while (!to_delete.empty()) {
            auto next = to_delete.front();
            to_delete.pop();

            if (next == _fake)
                continue;

            to_delete.push(next->_left);
            to_delete.push(next->_right);

            delete next;
        }

        delete[] reinterpret_cast<char *>(_fake);
    }

private:

    inline void _push(const T &value) {
        auto new_node = new node{value, nullptr, _fake, _fake};

        if (_root == _fake) {
            new_node->_parent = _fake;
            _fake->_left = new_node;
            _fake->_right = new_node;
            _root = new_node;
            return;
        }

        auto current = _root;

        while (true) {
            if (_cmp(value, current->_data)) {
                if (current->_left == _fake) {
                    current->_left = new_node;
                    break;
                } else {
                    current = current->_left;
                }
            } else {
                if (current->_right == _fake) {
                    current->_right = new_node;
                    break;
                } else {
                    current = current->_right;
                }
            }
        }

        new_node->_parent = current;

        if (_fake->_left == current && _fake->_left != _fake) {
            _fake->_left = new_node;
        }

        if (_fake->_right == current && _fake->_right != _fake) {
            _fake->_right = new_node;
        }

        _balance();
    }

    inline void _balance() {

    }

    inline bool _find(const T &value) const {
        auto current = _root;

        while (current != _fake) {
            if (_eq(current->_data, value))
                return true;

            if (_cmp(value, current->_data)) {
                current = current->_left;
            } else {
                current = current->_right;
            }
        }

        return false;
    }

    inline bool _eq(const T &a, const T &b) const {
        return (!_cmp(a, b) && !_cmp(b, a));
    }
};

#endif //TASK4_AVL_H
