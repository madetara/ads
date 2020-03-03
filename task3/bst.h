//
// Created by dv.kab on 2/26/2020.
//

#ifndef TASK3_BST_H
#define TASK3_BST_H

#include <functional>
#include <queue>
#include <stack>
#include <ostream>

template <typename T, typename Cmp = std::less<T>>
struct bst {
private:
    struct node
    {
        const T _value;
        node *_parent;
        node *_left;
        node *_right;
        bool _pinned;

        explicit node(T value) : _value(value), _left(nullptr), _right(nullptr), _parent(nullptr), _pinned(false) {}

        void pin() {
            _pinned = true;
        }

        void unpin() {
            _pinned = false;
        }

        ~node() {
            if (_pinned)
                throw std::logic_error("trying to delete pinned node");
        }
    };
public:
    class iterator {
        node *_current;
        bst *_owner;
        typedef ptrdiff_t difference_type;
        typedef T value_type;
        typedef T& reference;
        typedef T* pointer;
        typedef std::bidirectional_iterator_tag iterator_category;

        void swap(iterator &other) {
            std::swap(this->_current, other._current);
        }

    public:
        iterator(bst *owner, node *node) : _owner(owner), _current(node) {
            _current->pin();
        }

        iterator &operator=(const iterator &other) {
            if (this == other)
                return *this;
            this->swap(other);
            return *this;
        }

        iterator &operator++() {
            auto next = _owner->_successor(_current);
            if (next != _current) {
                _current->unpin();
                _current = next;
                _current->pin();
            }

            return *this;
        }

        iterator operator++(int) {
            auto result = *this;
            ++(*this);
            return result;
        }

        iterator &operator--() {
            auto next = _owner->_predecessor(_current);
            if (next != _current) {
                _current->unpin();
                _current = next;
                _current->pin();
            }

            return *this;
        }

        iterator operator--(int) {
            auto result = *this;
            --(*this);
            return result;
        }

        value_type operator*() {
            return _current->_value;
        }

        pointer operator->() {
            return &(_current->_value);
        }

        bool operator==(const iterator &other) const {
            return this->_current == other._current;
        }

        bool operator!=(const iterator &other) const {
            return !(*this == other);
        }

        ~iterator() {
            _current->unpin();
        }
    };

    bst() : _cmp(Cmp()) {
        // fake_node required for end iterator
        auto fake_node = reinterpret_cast<node*>(new char[sizeof(node)]);
        fake_node->_parent = nullptr;
        fake_node->_left = nullptr;
        fake_node->_right = nullptr;

        _most_right = _most_left = _root = fake_node;
    }

    bst(const bst<T, Cmp> &other) : bst() {
        auto q = std::queue<node *>();
        q.push(other._root);

        while (!q.empty()) {
            auto current = q.front();
            q.pop();

            if (current == nullptr || current == other._most_right)
                continue;

            this->push(current->_value);
            q.push(current->_left);
            q.push(current->_right);
        }
    }

    bst(bst<T, Cmp> &&other) {
        this->_swap(other);
    }

    bst<T, Cmp> &operator=(const bst<T, Cmp> &other) {
        if (this != &other)
            this->_swap(other);

        return *this;
    }

    void push(const T value) {
        if (_root == _most_right) {
            _root = new node(value);
            _root->_right = _most_right;
            _most_right->_parent = _root;
            _most_left = _root;
            return;
        }

        auto current = _root;

        while (true) {
            if (_cmp(current->_value, value)) {
                if (current->_right == nullptr || current->_right == _most_right) {
                    auto next_node = new node(value);
                    next_node->_parent = current;

                    if (current->_right == _most_right) {
                        _most_right->_parent = next_node;
                        next_node->_right = _most_right;
                    }

                    current->_right = next_node;
                    break;
                }
                current = current->_right;
            } else {
                if (current->_left == nullptr) {
                    current->_left = new node(value);
                    current->_left->_parent = current;

                    if (current == _most_left)
                        _most_left = current->_left;

                    break;
                }
                current = current->_left;
            }
        }
    }

    bool find(const T &value) const {
        return _find(value) != nullptr;
    }

    T min() const {
        _ensure_not_empty();
        return _most_left->_value;
    }

    T max() const {
        _ensure_not_empty();
        return _most_right->_parent->_value;
    }

    T next_greater_or_same(const T &value) const {
        _ensure_not_empty();
        auto target = _find(value);

        if (target == nullptr || target == _most_right->_parent) {
            auto m = max();
            return _cmp(value, m) ? m : value;
        }

        return _successor(target)->_value;
    }

    T next_less_or_same(const T &value) const {
        _ensure_not_empty();
        auto target = _find(value);

        if (target == nullptr) {
            auto m = min();
            return _cmp(value, m) ? value : m;
        }

        return _predecessor(target)->_value;
    }

    void remove(const T &value) {
        if (is_empty())
            return;

        auto to_remove = _find(value);

        if (to_remove == nullptr)
            return;

        if (_is_leaf(to_remove)) {
            _remove_leaf(to_remove);
            return;
        }

        if (_is_root(to_remove)) {
            _remove_root();
            return;
        }

        auto left = to_remove->_left;
        auto right = to_remove->_right;
        auto parent = to_remove->_parent;

        if (parent->_left == to_remove) {
            _reattach_branch(right, left);
        } else {
            _reattach_branch(left, right);
        }

        delete to_remove;
    }

    inline bool is_empty() const {
        return _most_left == _most_right;
    }

    inline iterator begin() {
        return iterator(this, _most_left);
    }

    inline iterator end() {
        return iterator(this, _most_right);
    }

    void infix_print(std::ostream &out) const {
        if (is_empty())
            out << "Empty!";
        else
            _infix_walk(out, _root);

        out << std::endl;
    }

    void infix_print_reverse(std::ostream &out) const {
        if (is_empty()) {
            out << "Empty!" << std::endl;
            return;
        }

        auto to_print = std::stack<node*>();
        to_print.push(_root);

        auto current = _root->_right;

        while (!to_print.empty() || current != nullptr) {
            if (current != _most_right && current != nullptr) {
                to_print.push(current);
                current = current->_right;
            } else {
                current = to_print.top();
                to_print.pop();
                out << current->_value << " ";

                current = current->_left;
            }
        }

        out << std::endl;
    }

    void layered_print(std::ostream &out) const {
        if (is_empty()) {
            out << "Empty!" << std::endl;
            return;
        }

        auto q = std::queue<std::pair<node *, int>>();
        auto current_layer = 0;
        q.push(std::make_pair(_root, 0));

        while (!q.empty()) {
            auto current = q.front();
            q.pop();

            if (current.first == nullptr || current.first == _most_right)
                continue;

            if (current.second > current_layer) {
                out << std::endl;
                current_layer = current.second;
            }

            q.push(std::make_pair(current.first->_left, current_layer + 1));
            q.push(std::make_pair(current.first->_right, current_layer + 1));

            out << current.first->_value << " ";
        }

        out << std::endl;
    }

    bool equals(const bst<T, Cmp> &other) const {
        if (is_empty() && other.is_empty())
            return true;

        return _equals(_root, _most_right, other._root, other._most_right);
    }

    ~bst() {
        auto to_delete = std::queue<node*>();
        to_delete.push(_root);

        while (!to_delete.empty()) {
            auto current = to_delete.front();
            to_delete.pop();
            if (current == nullptr || current == _most_right)
                continue;

            to_delete.push(current->_left);
            to_delete.push(current->_right);
            delete current;
        }

        delete[] reinterpret_cast<char*>(_most_right);
        _root = _most_right = _most_left = nullptr;
    }


private:
    Cmp _cmp;
    node *_root;
    node *_most_right;
    node *_most_left;

    inline bool _is_leaf(node *node) const {
        return node->_parent != nullptr &&
               node->_left == nullptr &&
               (node->_right == nullptr || node->_right == _most_right);
    }

    inline bool _is_root(node *node) const {
        return node == _root;
    }

    // returns deepest node with requested value or nullptr if node not found
    node *_find(const T &value) const {
        auto current = _root;
        node *result = nullptr;

        while (current != nullptr && current != _most_right) {
            if (!_cmp(current->_value, value) && !_cmp(value, current->_value))
                result = current;

            if (_cmp(current->_value, value)) {
                current = current->_right;
            } else {
                current = current->_left;
            }
        }

        return result;
    }

    // returns next node
    // it can be the same as starting node
    node *_successor(node *node) const {
        if (node == _most_right)
            return node;

        auto current = node;

        if (current->_right != nullptr) {
            current = current->_right;

            while (current->_left != nullptr)
                current = current->_left;

            return current;
        }

        while (current->_parent != nullptr && current->_parent->_right == current)
            current = current->_parent;

        if (current->_parent != nullptr)
            return current->_parent;

        return current;
    }

    // returns previous node
    // it can be same as starting node
    node *_predecessor(node *node) const {
        if (node == _most_left)
            return node;

        auto current = node;

        if (current->_left != nullptr) {
            current = current->_left;

            while (current->_right != nullptr)
                current = current->_right;

            return current;
        }

        while (current->_parent != nullptr && current->_parent->_left == current)
            current = current->_parent;

        if (current->_parent != nullptr)
            return current->_parent;

        return current;
    }

    inline void _remove_leaf(node *node) {
        auto parent = node->_parent;

        if (parent->_left == node) {
            parent->_left = nullptr;
        } else if (node->_right == _most_right) {
            parent->_right = _most_right;
            _most_right->_parent = parent;
        } else {
            parent->_right = nullptr;
        }

        delete node;
    }

    inline void _remove_root() {
        auto to_remove = _root;

        auto right = _root->_right;
        auto left = _root->_left;

        if (right == _most_right) {
            if (left == nullptr) {
                _root = _most_left = _most_right;
            } else {
                _root = left;
                _sink_right(left, right);
            }
        } else {
            _root = right;
            if (left != nullptr)
                _sink_left(right, left);
            else
                _most_left = _root;
        }

        delete to_remove;
    }

    inline void _reattach_branch(node *new_branch, node *dead_branch) {
        auto to_remove = dead_branch->_parent;
        if (new_branch != nullptr) {
            _replace_child(to_remove, new_branch);
            _sink_left(dead_branch, new_branch);
        } else {
            _replace_child(to_remove, dead_branch);
        }
    }

    inline void _replace_child(node *child, node *replacement) {
        auto parent = child->_parent;
        if (parent->_left == child)
            parent->_left = replacement;
        else
            parent->_right = replacement;

        replacement->_parent = parent;
    }

    inline void _sink_left(node *lake, node *victim) {
        auto current = lake;
        while (current->_left != nullptr)
            current = current->_left;

        if (current == _most_left)
            _most_left = victim;

        current->_left = victim;
        victim->_parent = current;
    }

    inline void _sink_right(node *lake, node *victim) {
        auto current = lake;
        while (current->_right != nullptr)
            current = current->_right;

        current->_right = victim;
        victim->_parent = current;
    }

    inline void _ensure_not_empty() const {
        if (is_empty())
            throw std::logic_error("tree is empty");
    }

    void _infix_walk(std::ostream &out, node *node) const {
        if (node == nullptr || node == _most_right)
            return;
        _infix_walk(out, node->_left);
        out << node->_value << " ";
        _infix_walk(out, node->_right);
    }

    bool _equals(node *this_, node *this_most_right, node *other, node *other_most_right) const {
        if (this_ == other)
            return true;

        if (this_ == this_most_right && other == other_most_right)
            return true;

        auto value_equal =
                !_cmp(this_->_value, other->_value) &&
                !_cmp(other->_value, this_->_value);

        return value_equal &&
               _equals(this_->_left, this_most_right, other->_left, other_most_right) &&
               _equals(this_->_right, this_most_right, other->_right, other_most_right);
    }

    void _swap(const bst<T, Cmp> &other) {
        std::swap(this->_root, other._root);
        std::swap(this->_most_right, other._most_right);
        std::swap(this->_most_left, other._most_left);
    }
};

#endif //TASK3_BST_H
