//
// Created by dv.kab on 2/19/2020.
//

#ifndef ASD2_NODE_H
#define ASD2_NODE_H

template<typename T>
struct Node {
    T value;
    Node<T> *next;
    Node<T> *prev;

    explicit Node(T value, Node<T> *next = nullptr, Node<T> *prev = nullptr):
        value(value), next(next), prev(prev) {}

    virtual bool isEnd() const {
        return false;
    }

protected:
    Node(): Node(T()) {}
};

template <typename T>
struct EndNode: public Node<T> {
    bool isEnd() const override {
        return true;
    }
};

#endif //ASD2_NODE_H
