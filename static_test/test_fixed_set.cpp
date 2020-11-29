#include <cstring>
#include <iostream>
#include <random>
#include <set>
#include <stdexcept>
#include <vector>

#include "fixed set.cpp"

std::vector<int> ReadSequence() {
  size_t size;
  std::cin >> size;
  std::vector<int> sequence(size);
  for (auto &current : sequence) {
    std::cin >> current;
  }
  return sequence;
}

std::vector<bool> PerformRequests(const std::vector<int> &requests,
                                  const FixedSet &set) {
  std::vector<bool> request_answers;
  request_answers.reserve(requests.size());
  for (int request : requests) {
    request_answers.push_back(set.Contains(request));
  }
  return request_answers;
}

void PrintRequestsResponse(const std::vector<bool> &request_answers) {
  for (bool answer : request_answers) {
    std::cout << (answer ? "Yes" : "No") << "\n";
  }
}

void RunTests();

int main(int argc, char **argv) {
  if (argc > 1 && !strcmp(argv[1], "--testing")) {
    RunTests();
    return 0;
  }

  std::ios::sync_with_stdio(false);

  auto numbers = ReadSequence();
  auto requests = ReadSequence();
  FixedSet set;
  set.Initialize(numbers);
  PrintRequestsResponse(PerformRequests(requests, set));

  return 0;
}

// ========= TESTING ZONE =========

#define ASSERT_EQ(expected, actual)                                            \
  do {                                                                         \
    auto __expected = expected;                                                \
    auto __actual = actual;                                                    \
    if (!(__expected == __actual)) {                                           \
      std::cerr << __FILE__ << ":" << __LINE__ << ": Assertion error"          \
                << std::endl;                                                  \
      std::cerr << "\texpected: " << __expected << " (= " << #expected << ")"  \
                << std::endl;                                                  \
      std::cerr << "\tgot: " << __actual << " (= " << #actual << ")"           \
                << std::endl;                                                  \
      std::cerr << "=========== FAIL ===========\n";                           \
      throw std::runtime_error("Check failed");                                \
    }                                                                          \
  } while (false)

void Empty() {
  FixedSet set;
  set.Initialize({});
  ASSERT_EQ(false, set.Contains(0));
}

void Simple() {
  FixedSet set;
  set.Initialize({-3, 5, 0, 3, 7, 1, 123, 3, 3, 3, 123, 22});
  ASSERT_EQ(true, set.Contains(0));
  ASSERT_EQ(true, set.Contains(-3));
  ASSERT_EQ(true, set.Contains(1));
  ASSERT_EQ(false, set.Contains(2));
  ASSERT_EQ(true, set.Contains(22));
  ASSERT_EQ(false, set.Contains(4));
  ASSERT_EQ(true, set.Contains(22));
  ASSERT_EQ(true, set.Contains(22));
  ASSERT_EQ(false, set.Contains(4));
  ASSERT_EQ(false, set.Contains(-2));
  ASSERT_EQ(false, set.Contains(-14));
  ASSERT_EQ(false, set.Contains(4));
}

void RepeatInitialize() {
  FixedSet set;
  const int shift = 100;
  int element = 0;
  int last = -1;
  for (int elements_count = 0; elements_count < 10; ++elements_count) {
    std::vector<int> elements;
    for (int i = 0; i < elements_count; ++i) {
      elements.push_back(element++);
    }
    set.Initialize(elements);
    for (auto elem : elements) {
      ASSERT_EQ(true, set.Contains(elem));
    }
    ASSERT_EQ(false, set.Contains(last));
    last = element - 1;
    element += shift;
  }
}

void Magic() {
#ifdef MAGIC
  std::cerr << "You've been visited by Hash Police!\n";
  std::cerr << "Probably your hash table is not as good as you think.\n";
  std::cerr << "No ticket today, but you better be careful.\n\n";
  int first = -1'000'000'000;
  int second = first + MAGIC;
  FixedSet set;
  set.Initialize({first, second});
  ASSERT_EQ(true, set.Contains(first));
  ASSERT_EQ(true, set.Contains(second));
  ASSERT_EQ(false, set.Contains(0));
#endif
}

void StressTests() {
  std::mt19937 gen;
  std::uniform_int_distribution<int64_t> dist_n(1'00'00, 1'00'000);
  std::uniform_int_distribution<int64_t> dist_q(1'000'000, 1'000'000);
  std::uniform_int_distribution<int64_t> dist(-1'000'000'000, 1'000'000'000);
  for (int i = 0; i < 100; ++i) {
    int n = dist_n(gen);
    std::vector<int> vec(n);
    for (int j = 0; j < n; ++j) {
      vec[j] = dist(gen);
    }
    std::set<int> std_set;
    for (auto i : vec) {
      std_set.insert(i);
    }
    FixedSet my_set;
    my_set.Initialize(vec);
    for (int k = 0; k < 1'000; ++k) {
      int val = dist(gen);

      bool find;
      if (std_set.find(val) == std_set.end()) {
        find = false;
      } else {
        find = true;
      }

      if (find != my_set.Contains(val)) {
        std::cout << val << " " << my_set.Contains(val) << "\n";
        for (int j = 0; j < n; ++j) {
          std::cout << vec[j] << " ";
        }

        std::cout << "\nERROR\n";
        exit(-1);
      }
    }
    std::cout << i << "\n";
  }
}

void RunTests() {
  std::cerr << "Running tests...\n";
  Empty();
  Simple();
  RepeatInitialize();
  Magic();
  StressTests();
  std::cerr << "Tests are passed!\n";
}