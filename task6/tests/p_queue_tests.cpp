//
// Created by dv.kab on 3/25/2020.
//

#include "../lib/p_queue.h"

#include "gtest/gtest.h"

class p_queue_tests: public ::testing::Test {
public:
    using p_q = p_queue<int>;
};

TEST_F(p_queue_tests, empty_on_create) {
    p_q q;
    ASSERT_TRUE(q.empty());
    ASSERT_EQ(q.size(), 0);
}

TEST_F(p_queue_tests, size_changed_on_push) {
    p_q q;
    q.push(1);

    ASSERT_EQ(q.size(), 1);
    ASSERT_FALSE(q.empty());
}

TEST_F(p_queue_tests, size_changed_after_pop) {
    p_q q;
    q.push(1);
    q.pop();

    ASSERT_EQ(q.size(), 0);
    ASSERT_TRUE(q.empty());
}

TEST_F(p_queue_tests, max_element_is_on_top) {
    p_q q;
    q.push(42);
    q.push(13);
    q.push(666);

    ASSERT_EQ(q.top(), 666);

    q.pop();
    ASSERT_EQ(q.top(), 42);

    q.pop();
    ASSERT_EQ(q.top(), 13);
}
