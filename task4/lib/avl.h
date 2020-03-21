//
// Created by dv.kab on 3/12/2020.
//

#ifndef TASK4_AVL_H
#define TASK4_AVL_H

#include <queue>
#include <functional>
#include <iterator>
#include <random>

template<typename T, typename Cmp = std::less<T>, typename Alloc = std::allocator<T>>
class avl {
private:
    struct node;

public:
    using size_type = size_t;
    using alloc_type = typename std::allocator_traits<Alloc>::template rebind_alloc<node>;

    class iterator;

    using key_type = T;
    using key_compare = Cmp;
    using value_compare = Cmp;
    using value_type = T;
    using allocator_type = alloc_type;
    using difference_type = size_t;
    using pointer = key_type *;
    using const_pointer = const pointer;
    using reference_type = value_type &;
    using const_reference = const value_type &;
    using const_iterator = iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    class iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = avl::difference_type;
        using value_type = avl::value_type;
        using reference = avl::const_reference;
        using pointer = avl::const_pointer;

    private:
        node *_data;

        iterator(node *current) : _data(current) {}

        friend class avl;

    public:
        iterator(const iterator &other) : iterator(other._data) {}

        reference operator*() const {
            return _data->_data;
        }

        pointer operator->() const {
            return &(_data->_data);
        }

        bool operator==(const iterator &other) const {
            return _data == other._data;
        }

        bool operator!=(const iterator &other) const {
            return !(*this == other);
        }

        iterator &operator++() {
            _data = _next_node();
            return *this;
        }

        iterator operator++(int) {
            auto result = *this;
            ++(*this);
            return result;
        }

        iterator &operator--() {
            _data = _prev_node();
            return *this;
        }

        iterator operator--(int) {
            auto result = *this;
            --(*this);
            return result;
        }

    private:
        node *_next_node() const {
            auto current = _data;

            if (avl::_is_fake(current)) {
                return current->_left;
            }

            if (avl::_is_fake(current->_right) && current->_right->_right == current) {
                return current->_right;
            }

            if (!avl::_is_fake(current->_right)) {
                current = current->_right;

                while (!avl::_is_fake(current->_left))
                    current = current->_left;

                return current;
            }

            while (!avl::_is_fake(current->_parent) && current->_parent->_right == current)
                current = current->_parent;

            if (!avl::_is_fake(current->_parent))
                return current->_parent;

            return current;
        }


        node *_prev_node() const {
            auto current = _data;

            if (avl::_is_fake(current)) {
                return current->_right;
            }

            if (avl::_is_fake(current->_left) && current->_left->_left == current) {
                return current->_left;
            }

            if (!avl::_is_fake(current->_left)) {
                current = current->_left;

                while (!avl::_is_fake(current->_right))
                    current = current->_right;

                return current;
            }

            while (!avl::_is_fake(current->_parent) && current->_parent->_left == current)
                current = current->_parent;

            if (!avl::_is_fake(current->_parent))
                return current->_parent;

            return current;
        }

        inline bool _is_leaf() const {
            return avl::_is_leaf(_data);
        }

        inline bool _is_fake() const {
            return avl::_is_fake(_data);
        }

        inline bool _has_single_child() const {
            return avl::_has_single_child(_data);
        }
    };

private:
    struct node {
        T _data;
        node *_parent;
        node *_left;
        node *_right;

        node(T data = T(), node *parent = nullptr, node *left = nullptr, node *right = nullptr) :
                _data(data), _parent(parent), _left(left), _right(right) {}
    };


    Cmp _cmp;
    alloc_type _al;
    size_type _size;

    node *_root;
    node *_fake;

public:

    avl(Cmp cmp = Cmp(), alloc_type al = alloc_type()) : _cmp(cmp), _al(al), _size(0) {
        _fake = _make_fake();
        _root = _fake;
    }

    avl(const avl<T, Cmp, Alloc> &other) : _cmp(Cmp(other._cmp)), _al(Alloc(other._al)), _size(other._size) {
        _fake = _make_fake();
        _fake->_left = _fake->_right = _fake->_parent = _fake;

        if (other.size() == 0) {
            _root = _fake;
            return;
        }

        _root = _make_node(T(other._root->_data), _fake, _fake, _fake);
        _fake->_left = _fake->_right = _root;
        _copy(other._root, other._fake, _root, _fake);
    }

    avl(avl<T, Cmp, Alloc> &&other) : avl() {
        this->_swap(other);
    }

    template<typename It>
    avl(It begin, It end, Cmp cmp = Cmp(), alloc_type al = alloc_type()) : avl(cmp, al) {
        auto current = begin;
        while (current != end) {
            this->insert(*(current++));
        }
    }

    avl(std::initializer_list<T> values) : avl() {
        for (const auto &value: values) {
            this->insert(value);
        }
    }


    iterator find(const T &value) const {
        return _find(value);
    }

    size_type size() const {
        return _size;
    }

    alloc_type get_allocator() const {
        return _al;
    }

    bool empty() const {
        return size() == 0;
    }

    iterator begin() const {
        return iterator(_fake->_left);
    }

    reverse_iterator rbegin() const {
        return reverse_iterator(end());
    }

    iterator end() const {
        return iterator(_fake);
    }

    reverse_iterator rend() const {
        return reverse_iterator(begin());
    }

    size_type count(const T &value) const {
        return _count(value);
    }

    void swap(avl<T, Cmp, Alloc> &other) {
        this->_swap(other);
    }

    avl<T, Cmp, Alloc> &operator=(const avl<T, Cmp, Alloc> &other) {
        if (this == &other)
            return *this;

        avl<T, Cmp, Alloc> temp(other);
        this->_swap(temp);
        return *this;
    }

    bool operator==(const avl<T, Cmp, Alloc> &other) const {
        auto it1 = this->begin(), it2 = other.begin();

        while (it1 != end() && it2 != other.end() && _eq(*it1, *it2)) {
            ++it1;
            ++it2;
        }

        return it1 == this->end() && it2 == other.end();
    }

    bool operator!=(const avl<T, Cmp, Alloc> &other) const {
        return !(*this == other);
    }

    bool operator<(const avl<T, Cmp, Alloc> &other) const {
        auto it1 = this->begin(), it2 = other.begin();

        while (it1 != this->end() && it2 != other.end() && _eq(*it1, *it2)) {
            ++it1;
            ++it2;
        }

        if (it1 == this->end())
            return it2 != other.end();

        return it2 != other.end() && *it1 < *it2;
    }

    bool operator>=(const avl<T, Cmp, Alloc> &other) const {
        return !(*this < other);
    }

    bool operator>(const avl<T, Cmp, Alloc> &other) const {
        return other < *this;
    }

    bool operator<=(const avl<T, Cmp, Alloc> &other) const {
        return !(*this > other);
    }

    key_compare key_comp() const {
        return _cmp;
    }

    value_compare value_comp() const {
        return _cmp;
    }

    const_iterator insert(const T &value) {
        auto result = _push(value);
        ++_size;
        return result;
    }

    const_iterator insert(const_iterator it, const T &value) {
        auto current = it;
        auto prev = it;
        ++_size;

        if (current._is_fake() || _cmp(value, *current)) {
            --prev;

            while (!current._is_fake() && !_cmp(value, *current)) {
                current = prev--;
            }
        } else {
            while (!current._is_fake() && !_cmp(value, *current)) {
                prev = current++;
            }
        }

        // empty tree
        if (current == prev) {
            auto new_node = _make_node(value, _fake, _fake, _fake);
            _fake->_parent = _fake->_left = _fake->_right = new_node;
            return const_iterator(new_node);
        }

        // most left insert
        if (prev._is_fake()) {
            auto p_node = current._data;
            p_node->_left = _make_node(value, p_node, _fake, _fake);
            _fake->_left = p_node->_left;
            return const_iterator(p_node->_left);
        }

        // empty right insert
        if (_is_fake(prev._data->_right)) {
            prev._data->_right = _make_node(value, prev._data, _fake, _fake);
            if (_fake->_right == prev._data) {
                _fake->_right = prev._data->_right;
            }

            return const_iterator(prev._data->_right);
        }

        // non-empty right insert
        current._data->_left = _make_node(value, current._data, _fake, _fake);
        return const_iterator(current._data->_left);
    }

    template<typename It>
    void insert(It begin, It end) {
        auto current = begin;

        while (current != end) {
            this->insert(*(current++));
        }
    }

    const_iterator lower_bound(const T &value) const {
        auto current = _root, result = _root;

        while (!_is_fake(current)) {
            if (!_cmp(value, current->_data)) {
                result = current;
                current = current->_right;
            } else {
                current = current->_left;
            }
        }

        return const_iterator(result);
    }

    const_iterator upper_bound(const T &value) const {
        auto current = _root, result = _root;

        while (!_is_fake(current)) {
            if (_cmp(value, current->_data)) {
                result = current;
                current = current->_left;
            } else {
                current = current->_right;
            }
        }

        return const_iterator(result);
    }

    size_type erase(const T &target) {
        size_type removed = 0;

        auto current = find(target);
        while (current != end()) {
            erase(current);
            ++removed;
            current = find(target);
        }

        return removed;
    }

    iterator erase(iterator target) {
        if (target._is_fake())
            return _fake;

        if (target._is_leaf()) {
            iterator result(target);
            ++result;
            _erase_leaf(target);
            return result;
        }

        node *left_replace = nullptr;
        if (target == _fake->_left) {
            left_replace = (++iterator(target))._data;
        }

        node *right_replace = nullptr;
        if (target == _fake->_right) {
            right_replace = (--iterator(target))._data;
        }


        if (target._has_single_child()) {
            iterator result(target);
            ++result;

            auto parent = target._data->_parent;
            node *new_child;

            if (_is_fake(target._data->_left)) {
                new_child = target._data->_right;
                new_child->_parent = parent;
                if (parent->_left == target._data) {
                    parent->_left = new_child;
                } else {
                    parent->_right = new_child;
                }
            } else {
                new_child = target._data->_left;
                new_child->_parent = parent;
                if (parent->_left == target._data) {
                    parent->_left = new_child;
                } else {
                    parent->_right = new_child;
                }
            }

            if (_root == target._data) {
                _root = new_child;
            }

            if (left_replace != nullptr) {
                _fake->_left = left_replace;
            }

            if (right_replace != nullptr) {
                _fake->_right = right_replace;
            }

            _delete_node(target._data);
            return result;
        }

        iterator result(target), replacement(target);
        ++result;

        if (_coin_flip()) {
            ++replacement;
        } else {
            --replacement;
        }

        node *orphan(_fake);
        if (replacement._has_single_child()) {
            orphan = _is_fake(replacement._data->_right) ? replacement._data->_left : replacement._data->_right;
        }

        std::swap(target._data->_data, replacement._data->_data);

        if (orphan != _fake) {
            orphan->_parent = replacement._data->_parent;
            if (orphan->_parent->_left == replacement._data) {
                orphan->_parent->_left = orphan;
            } else {
                orphan->_parent->_right = orphan;
            }
        }

        _erase_leaf(replacement);
        return result;
    }

    iterator erase(iterator begin, iterator end) {
        auto current = begin;
        auto result = this->end();

        while (current != end) {
            result = erase(current++);
        }

        return result;
    }

    std::pair<const_iterator, const_iterator> equal_range(const T &value) const {
        auto begin = lower_bound(value);
        auto end = upper_bound(value);

        return std::make_pair(begin, end);
    }

    void clear() {
        _clear();
        _root = _fake;
        _size = 0;
    }

    ~avl() {
        _clear();
        _delete_fake(_fake);
    }

private:

    inline iterator _push(const T &value) {
        auto new_node = _make_node(value, _fake, _fake, _fake);
        auto result = iterator(new_node);

        if (_root == _fake) {
            new_node->_parent = _fake;
            _fake->_left = new_node;
            _fake->_right = new_node;
            _root = new_node;
            return result;
        }

        auto current = _root;
        auto going_left = true, going_right = true;

        while (true) {
            if (_cmp(value, current->_data)) {
                going_right = false;
                if (current->_left == _fake) {
                    current->_left = new_node;
                    break;
                } else {
                    current = current->_left;
                }
            } else {
                going_left = false;
                if (current->_right == _fake) {
                    current->_right = new_node;
                    break;
                } else {
                    current = current->_right;
                }
            }
        }

        new_node->_parent = current;

        if (going_left) {
            _fake->_left = new_node;
        }

        if (going_right) {
            _fake->_right = new_node;
        }

        _balance();

        return result;
    }

    inline void _balance() {

    }

    inline iterator _find(const T &value) const {
        auto current = _root;

        while (current != _fake) {
            if (_eq(current->_data, value))
                return iterator(current);

            if (_cmp(value, current->_data)) {
                current = current->_left;
            } else {
                current = current->_right;
            }
        }

        return end();
    }

    inline bool _eq(const T &a, const T &b) const {
        return (!_cmp(a, b) && !_cmp(b, a));
    }

    void _clear() {
        auto to_delete = std::queue<node *>();
        to_delete.push(_root);

        while (!to_delete.empty()) {
            auto next = to_delete.front();
            to_delete.pop();

            if (next == _fake)
                continue;

            to_delete.push(next->_left);
            to_delete.push(next->_right);

            _delete_node(next);
        }
    }

    inline node *_make_fake() {
        auto fake = _al.allocate(1);

        std::allocator_traits<alloc_type>::construct(_al, &(fake->_parent));
        fake->_parent = fake;

        std::allocator_traits<alloc_type>::construct(_al, &(fake->_left));
        fake->_left = fake;

        std::allocator_traits<alloc_type>::construct(_al, &(fake->_right));
        fake->_right = fake;

        return fake;
    }

    inline node *_make_node(const T &data, node *parent, node *left, node *right) {
        auto new_node = _al.allocate(1);

        std::allocator_traits<alloc_type>::construct(_al, &(new_node->_parent));
        new_node->_parent = parent;

        std::allocator_traits<alloc_type>::construct(_al, &(new_node->_left));
        new_node->_left = left;

        std::allocator_traits<alloc_type>::construct(_al, &(new_node->_right));
        new_node->_right = right;

        std::allocator_traits<alloc_type>::construct(_al, &(new_node->_data));
        new_node->_data = data;

        return new_node;
    }

    inline void _delete_fake(node *node) {
        std::allocator_traits<alloc_type>::destroy(_al, &(node->_parent));
        std::allocator_traits<alloc_type>::destroy(_al, &(node->_left));
        std::allocator_traits<alloc_type>::destroy(_al, &(node->_right));
        std::allocator_traits<alloc_type>::deallocate(_al, node, 1);
    }

    inline void _delete_node(node *node) {
        std::allocator_traits<alloc_type>::destroy(_al, &(node->_data));
        --_size;
        _delete_fake(node);
    }

    void _copy(node *from, node *from_fake, node *to, node *to_fake) {
        if (to == to_fake)
            return;

        auto left = from->_left == from_fake ?
                    to_fake :
                    _make_node(T(from->_left->_data), to, nullptr, nullptr);

        auto right = from->_right == from_fake ?
                    to_fake :
                    _make_node(T(from->_right->_data), to, nullptr, nullptr);

        if (to_fake->_left == to && left != to_fake) {
            to_fake->_left = left;
        }

        if (to_fake->_right == to && right != to_fake) {
            to_fake->_right = right;
        }

        to->_left = left;
        to->_right = right;

        _copy(from->_left, from_fake, to->_left, to_fake);
        _copy(from->_right, from_fake, to->_right, to_fake);
    }

    size_type _count(const T &value) const {
        auto current = _root;
        size_type result = 0;

        while (!_is_fake(current)) {
            if (_eq(current->_data, value))
                ++result;

            if (_cmp(value, current->_data))
                current = current->_left;
            else
                current = current->_right;
        }

        return result;
    }

    void _swap(avl<T, Cmp, Alloc> &other) {
        std::swap(_fake, other._fake);
        std::swap(_root, other._root);
        std::swap(_size, other._size);
        std::swap(_al, other._al);
        std::swap(_cmp, other._cmp);
    }

    void _erase_leaf(iterator leaf) {
        auto leaf_node = leaf._data;
        auto parent = leaf_node->_parent;

        if (parent->_left == leaf_node) {
            parent->_left = _fake;
        } else {
            parent->_right = _fake;
        }

        if (_root == leaf_node) {
            _root = _fake->_left = _fake->_right = _fake;
        }

        _delete_node(leaf_node);
    }

    static inline bool _has_single_child(node *node) {
        return _is_fake(node->_right) ^ _is_fake(node->_left);
    }

    static inline bool _is_leaf(node *node) {
        return _is_fake(node->_left) && _is_fake(node->_right);
    }

    static inline bool _is_fake(node *node) {
        return node->_parent == node;
    }

    static inline bool _coin_flip() {
        return rand() % 2 == 0;
    }
};

#endif //TASK4_AVL_H
