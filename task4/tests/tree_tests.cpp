//
// Created by dv.kab on 3/11/2020.
//

#include <gtest/gtest.h>

#include "../lib/avl.h"


class tree_test : public ::testing::Test {
public:
    template<typename T> using Myal = std::allocator<T>;
    template<typename T> using Mypred = std::less<T>;

//    template<typename T> using ContainerTemplate = std::set<T, Mypred<T>, Myal<T>>;
    template <typename T> using ContainerTemplate = avl<T, Mypred<T>, Myal<T>>;

    using Mycont = ContainerTemplate<char>;
protected:
};

TEST_F(tree_test, TreeSizeTest) {
    ContainerTemplate<int> T1;
    ASSERT_EQ(T1.size(), ContainerTemplate<int>::size_type(0));
    ContainerTemplate<int> Tree = {40, 50, 30, 35, 10, 75, 23, 87, 68};
    ASSERT_EQ(Tree.size(), ContainerTemplate<int>::size_type(9));
    ContainerTemplate<int> Tree2(Tree);
    ASSERT_EQ(Tree.size(), Tree2.size());
}

TEST_F(tree_test, TreeEqualityTest) {
    ContainerTemplate<int> T1;
    ASSERT_EQ(T1.size(), ContainerTemplate<int>::size_type(0));
    ContainerTemplate<int> Tree = {40, 50, 30, 35, 10, 75, 23, 87, 68};
    ASSERT_EQ(Tree.size(), ContainerTemplate<int>::size_type(9));
    ContainerTemplate<int> Tree2(Tree);
    ASSERT_TRUE(Tree.size() == Tree2.size());
}

TEST_F(tree_test, TreeEqualityTest2) {
    ContainerTemplate<int> T1;
    ASSERT_EQ(T1.size(), ContainerTemplate<int>::size_type(0));
    ContainerTemplate<int> Tree = {40, 50, 30, 35, 10, 75, 23, 87, 68};
    ASSERT_EQ(Tree.size(), ContainerTemplate<int>::size_type(9));
    ContainerTemplate<int> Tree2(Tree);
    ASSERT_EQ(Tree.size(), Tree2.size());
}
