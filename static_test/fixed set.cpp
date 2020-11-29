#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <utility>
#include <math.h>
#include <optional>

class LinearHash {
public:
    int64_t operator()(int value) const {
        auto hash = (alpha_ * value + beta_) % primary_number_;
        if (hash < 0) {
            hash += primary_number_;
        }
        return hash;
    }

    LinearHash(int64_t alpha, int64_t beta, int64_t primary_number)
        : alpha_(alpha), beta_(beta), primary_number_(primary_number) {
    }

    LinearHash() {
    }

    ~LinearHash() {
    }

private:
    int64_t alpha_;
    int64_t beta_;
    int64_t primary_number_;
};

size_t SumOfSquares(const std::vector<int64_t>& array) {
    size_t sum = 0;
    for (auto i : array) {
        sum += i * i;
    }

    return sum;
}

LinearHash GenerateLinearHash(std::mt19937& gen, int64_t primary_number) {
    std::uniform_int_distribution<int64_t> dist(1, primary_number);
    int64_t alpha = dist(gen), beta = dist(gen);

    return LinearHash(alpha, beta, primary_number);
}

std::vector<int> GetUniqueValuesVector(const std::vector<int>& values) {
    std::vector<int> unique_values(values);
    std::sort(unique_values.begin(), unique_values.end());

    auto last = std::unique(unique_values.begin(), unique_values.end());
    unique_values.erase(last, unique_values.end());

    return unique_values;
}

class ExternalHashTable {
private:
    void GenerateHashFuncWithBoundedSumOfSquares(const std::vector<int>& unique_numbers,
                                                 std::mt19937& gen) {
        size_t sum_of_squares_of_buckets_lenghts;
        number_of_buckets_ = unique_numbers.size();

        do {
            hash_func_ = GenerateLinearHash(gen, primary_number_);
            std::vector<int64_t> buckets_sizes(number_of_buckets_, 0);

            for (auto value : unique_numbers) {
                auto hashed_value = hash_func_(value) % unique_numbers.size();
                ++buckets_sizes[hashed_value];
            }
            sum_of_squares_of_buckets_lenghts = SumOfSquares(buckets_sizes);
        } while (4 * number_of_buckets_ < sum_of_squares_of_buckets_lenghts);
    }

    void Clear() {
        number_of_buckets_ = 0;
        buckets_.resize(0);
    }

public:
    explicit ExternalHashTable(int primary_number = 2'000'000'003)
        : primary_number_(primary_number) {
    }

    void Initialize(const std::vector<int>& unique_numbers, std::mt19937& gen) {
        GenerateHashFuncWithBoundedSumOfSquares(unique_numbers, gen);
    }

    bool Contains(int value) const {
        auto hashed_value = hash_func_(value) % number_of_buckets_;
        for (auto value_in_bucket : buckets_[hashed_value]) {
            if (value_in_bucket == value) {
                return true;
            }
        }
        return false;
    }
    int operator()(int value) const {
        if (number_of_buckets_ == 0) {
            throw std::runtime_error("Bad initialisation of external hash");
        }
        return hash_func_(value) % number_of_buckets_;
    }

private:
    LinearHash hash_func_;
    int primary_number_;
    size_t number_of_buckets_;
    std::vector<std::vector<int>> buckets_;
};

class InternalHashTable {
private:
    void GenerateNonCollisionHashFuncAndFillBuckets(const std::vector<int>& unique_values,
                                                    std::mt19937& gen) {
        size_t size = unique_values.size();
        buckets_.resize(size * size);
        number_of_buckets_ = size * size;
        bool exist_collision;
        std::uniform_int_distribution<int> dist(1, primary_number_);

        do {
            exist_collision = false;

            for (auto& i : buckets_) {
                i = std::nullopt;
            }

            hash_func_ = GenerateLinearHash(gen, primary_number_);

            for (auto value : unique_values) {
                auto hash = hash_func_(value) % buckets_.size();

                if (buckets_[hash] != std::nullopt) {
                    exist_collision = true;
                    break;
                } else {
                    buckets_[hash] = value;
                }
            }
        } while (exist_collision);
    }

public:
    void Initialize(const std::vector<int>& unique_numbers, std::mt19937& gen) {
        GenerateNonCollisionHashFuncAndFillBuckets(unique_numbers, gen);
    }

    explicit InternalHashTable(int primary_number = 2'000'000'003)
        : primary_number_(primary_number) {
    }

    bool Contains(int value) const {
        if (number_of_buckets_ == 0) {
            return false;
        }

        int hash_value = hash_func_(value) % number_of_buckets_;

        if (buckets_[hash_value] == std::nullopt) {
            return false;
        }

        return *(buckets_[hash_value]) == value;
    }

    int operator()(int value) {
        return hash_func_(value) % number_of_buckets_;
    }

private:
    LinearHash hash_func_;
    int primary_number_;
    size_t number_of_buckets_;
    std::vector<std::optional<int>> buckets_;
};

class FixedSet {
private:
    ExternalHashTable external_hash_table_;
    std::vector<InternalHashTable> internal_hash_tables_;
    std::mt19937 gen_;
    int64_t primary_number_;
    std::vector<int> unique_numbers_;

public:
    explicit FixedSet(int64_t primary_number = 2'000'000'003) : primary_number_(primary_number) {
    }

    void Clear() {
        internal_hash_tables_.resize(0);
        unique_numbers_.resize(0);
    }

    void Initialize(const std::vector<int>& numbers) {
        Clear();
        unique_numbers_ = GetUniqueValuesVector(numbers);

        external_hash_table_.Initialize(unique_numbers_, gen_);
        std::vector<std::vector<int>> collision_arrays(unique_numbers_.size());

        for (auto num : unique_numbers_) {
            auto external_hash = external_hash_table_(num);
            collision_arrays[external_hash].push_back(num);
        }

        for (const auto& collision_array : collision_arrays) {
            InternalHashTable hash_func;
            hash_func.Initialize(collision_array, gen_);
            internal_hash_tables_.emplace_back(hash_func);
        }
    }

    bool Contains(int number) const {

        if (unique_numbers_.empty()) {
            return false;
        }
        int64_t l1_index = external_hash_table_(number);
        const auto& l2_hash_tab = internal_hash_tables_[l1_index];

        return l2_hash_tab.Contains(number);
    }
};
