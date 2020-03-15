//
// Created by dv.kab on 3/15/2020.
//

#include <gtest/gtest.h>

class set_test : public ::testing::Test {
public:
    template<typename T> using Myal = std::allocator<T>;
    template<typename T> using Mypred = std::less<T>;

    template<typename T> using ContainerTemplate = std::set<T, Mypred<T>, Myal<T>>;

    using Mycont = ContainerTemplate<char>;
protected:
};

TEST_F(set_test, SetSizeTest) {
    Mycont v0;
    Myal<char> al = v0.get_allocator();
    Mypred<char> pred;
    Mycont v0a(pred), v0b(pred, al);
    ASSERT_TRUE(v0.empty() && v0.size() == 0);
    ASSERT_TRUE(v0a.size() == 0 && v0a.get_allocator() == al);
    ASSERT_TRUE(v0b.size() == 0 && v0b.get_allocator() == al);
}

TEST_F(set_test, SetCreationTest) {
    char carr[] = "abc", carr2[] = "def";
    Mycont v0;
    Myal<char> al = v0.get_allocator();
    Mypred<char> pred;
    Mycont v0a(pred);

    Mycont v1(carr, carr + 3);
    ASSERT_TRUE(v1.size() == 3 && *v1.begin() == 'a');

    Mycont v2(carr, carr + 3, pred);
    ASSERT_TRUE(v2.size() == 3 && *v2.begin() == 'a');

    Mycont v3(carr, carr + 3, pred, al);
    ASSERT_TRUE(v3.size() == 3 && *v3.begin() == 'a');

    const Mycont v4(carr, carr + 3);
    v0 = v4;
    ASSERT_TRUE(v1.size() == 3 && *v1.begin() == 'a');
}

TEST_F(set_test, SetIteratorTest) {
    char ch('a'), carr[] = "abc", carr2[] = "def";
    Mycont v0;
    Myal<char> al = v0.get_allocator();
    Mypred<char> pred;
    Mycont v0a(pred);

    Mycont v1(carr, carr + 3);
    Mycont v2(carr, carr + 3, pred);
    Mycont v3(carr, carr + 3, pred, al);
    const Mycont v4(carr, carr + 3);
    v0 = v4;

    Mycont::iterator p_it(v1.begin());
    Mycont::const_iterator p_cit(v4.begin());
    Mycont::reverse_iterator p_rit(v1.rbegin());
    Mycont::const_reverse_iterator p_crit(v4.rbegin());

    ASSERT_TRUE(*p_it == 'a' && *--(p_it = v1.end()) == 'c');
    ASSERT_TRUE(*p_cit == 'a' && *--(p_cit = v4.end()) == 'c');
    ASSERT_TRUE(*p_rit == 'c' && *--(p_rit = v1.rend()) == 'a');
    ASSERT_TRUE(*p_crit == 'c' && *--(p_crit = v4.rend()) == 'a');
}

TEST_F(set_test, SetInsertEraseTest) {
    char carr[] = "abc", carr2[] = "def";
    Mycont v0;
    Myal<char> al = v0.get_allocator();
    Mypred<char> pred;
    Mycont v0a(pred);

    Mycont v1(carr, carr + 3);
    Mycont v2(carr, carr + 3, pred);
    Mycont v3(carr, carr + 3, pred, al);
    const Mycont v4(carr, carr + 3);
    v0 = v4;

    Mycont::iterator p_it(v1.begin());
    Mycont::const_iterator p_cit(v4.begin());
    Mycont::reverse_iterator p_rit(v1.rbegin());
    Mycont::const_reverse_iterator p_crit(v4.rbegin());

    v0.clear();
    std::pair<Mycont::iterator, bool> pib = v0.insert('d');
    ASSERT_TRUE(*pib.first == 'd' && pib.second);
    ASSERT_TRUE(*--v0.end() == 'd');
    pib = v0.insert('d');
    ASSERT_TRUE(*pib.first == 'd' && !pib.second);
    ASSERT_TRUE(*v0.insert(v0.begin(), 'e') == 'e');
    v0.insert(carr, carr + 3);
    ASSERT_TRUE(v0.size() == 5 && *v0.begin() == 'a');
    v0.insert(carr2, carr2 + 3);
    ASSERT_TRUE(v0.size() == 6 && *--v0.end() == 'f');
    ASSERT_TRUE(*v0.erase(v0.begin()) == 'b' && v0.size() == 5);
    ASSERT_TRUE(*v0.erase(v0.begin(), ++v0.begin()) == 'c' && v0.size() == 4);
    ASSERT_TRUE(v0.erase('x') == 0 && v0.erase('e') == 1);
}

TEST_F(set_test, SetSwapAndCompTest) {
    char carr[] = "abc", carr2[] = "def";
    Mycont v0;
    Mycont v1(carr, carr + 3);

    v0.clear();
    std::pair<Mycont::iterator, bool> pib = v0.insert('d');
    pib = v0.insert('d');
    v0.insert(v0.begin(), 'e');
    v0.insert(carr, carr + 3);
    v0.insert(carr2, carr2 + 3);
    v0.erase(v0.begin());
    v0.erase(v0.begin(), ++v0.begin());
    v0.erase('x');
    v0.erase('e');
    v0.clear();
    ASSERT_TRUE(v0.empty());
    v0.swap(v1);
    ASSERT_TRUE(!v0.empty() && v1.empty());
    std::swap(v0, v1);
    ASSERT_TRUE(v0.empty() && !v1.empty());
    ASSERT_TRUE(v1 == v1 && v0 < v1);
    ASSERT_TRUE(v0 != v1 && v1 > v0);
    ASSERT_TRUE(v0 <= v1 && v1 >= v0);
}

TEST_F(set_test, SetComparatorTest) {
    Mycont v0;
    ASSERT_TRUE(v0.key_comp()('a', 'c') && !v0.key_comp()('a', 'a'));
    ASSERT_TRUE(v0.value_comp()('a', 'c') && !v0.value_comp()('a', 'a'));
}

TEST_F(set_test, SetAlgTest) {
    char carr[] = "abc";
    const Mycont v4(carr, carr + 3);

    ASSERT_TRUE(*v4.find('b') == 'b');
    ASSERT_TRUE(v4.count('x') == 0 && v4.count('b') == 1);
    ASSERT_TRUE(*v4.lower_bound('a') == 'a');
    ASSERT_TRUE(*v4.upper_bound('a') == 'b');
    std::pair<Mycont::const_iterator, Mycont::const_iterator> pcc = v4.equal_range('a');
    ASSERT_TRUE(*pcc.first == 'a' && *pcc.second == 'b');
}
