//
// Created by dv.kab on 3/25/2020.
//

#include "benchmark/benchmark.h"

#include "../lib/p_queue.h"
#include "../lib/p_queue_arr.h"

#include <vector>
#include <string>
#include <fstream>
#include <queue>

class length_compare {
public:
    bool operator()(const std::string &a, const std::string &b) const {
        return a.length() < b.length();
    }
};

class p_queue_fixture : public benchmark::Fixture {
public:
    std::vector<std::string> data;

    void SetUp(const ::benchmark::State &state) {
        std::ifstream in("data.txt", std::ios::in);
        std::string word;
        data = std::vector<std::string>();

        if (!in.good())
            throw std::logic_error("failed to read data");

        while (in >> word) {
            data.push_back(word);
        }
        in.close();
    }

    void TearDown(const ::benchmark::State &state) {
    }
};

BENCHMARK_DEFINE_F(p_queue_fixture, heap_based_priority_queue)(benchmark::State &state) {
    for (auto _ : state) {
        p_queue<std::string, std::vector<std::string>, length_compare> q;

        for (const auto &word: data) {
            q.push(word);
        }

        std::vector<std::string> top20;

        for (size_t i = 0; i < 20; ++i) {
            top20.push_back(q.top());
            q.pop();
        }
    }
}


BENCHMARK_DEFINE_F(p_queue_fixture, std_priority_queue)(benchmark::State &state) {
    for (auto _ : state) {
        std::priority_queue<std::string, std::vector<std::string>, length_compare> q;

        for (const auto &word: data) {
            q.push(word);
        }

        std::vector<std::string> top20;

        for (size_t i = 0; i < 20; ++i) {
            top20.push_back(q.top());
            q.pop();
        }
    }
}


BENCHMARK_DEFINE_F(p_queue_fixture, array_based_priority_queue)(benchmark::State &state) {
    for (auto _ : state) {
        p_queue_arr<std::string, std::vector<std::string>, length_compare> q;

        for (const auto &word: data) {
            q.push(word);
        }

        std::vector<std::string> top20;

        for (size_t i = 0; i < 20; ++i) {
            top20.push_back(q.top());
            q.pop();
        }
    }
}

BENCHMARK_REGISTER_F(p_queue_fixture, heap_based_priority_queue)->Unit(benchmark::kMillisecond);
BENCHMARK_REGISTER_F(p_queue_fixture, std_priority_queue)->Unit(benchmark::kMillisecond);
BENCHMARK_REGISTER_F(p_queue_fixture, array_based_priority_queue)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
