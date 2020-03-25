//
// Created by dv.kab on 3/25/2020.
//

#include "../lib/p_queue_arr.h"

#include "gtest/gtest.h"

class p_queue_arr_tests : public ::testing::Test {
public:
    using p_queue = p_queue_arr<int>;
};

TEST_F(p_queue_arr_tests, empty_on_create) {
    p_queue q;
    ASSERT_TRUE(q.empty());
    ASSERT_EQ(q.size(), 0);
}

TEST_F(p_queue_arr_tests, size_changed_on_push) {
    p_queue q;
    q.push(1);

    ASSERT_EQ(q.size(), 1);
    ASSERT_FALSE(q.empty());
}

TEST_F(p_queue_arr_tests, size_changed_after_pop) {
    p_queue q;
    q.push(1);
    q.pop();

    ASSERT_EQ(q.size(), 0);
    ASSERT_TRUE(q.empty());
}

TEST_F(p_queue_arr_tests, max_element_is_on_top) {
    p_queue q;
    q.push(42);
    q.push(13);
    q.push(666);

    ASSERT_EQ(q.top(), 666);

    q.pop();
    ASSERT_EQ(q.top(), 42);

    q.pop();
    ASSERT_EQ(q.top(), 13);
}