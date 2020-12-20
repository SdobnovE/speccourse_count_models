#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <utility>

struct Footballer;
struct FootballTeam;

template <class T>
class HeapSort;

void PrintMostEffectiveSolidaryTeam(std::vector<Footballer> footballers);
bool ComparatorIndexFootballer(const Footballer& left, const Footballer& right);
bool ComparatorEffectivityFootballer(const Footballer& left, const Footballer& right);
uint64_t GetEffectivityFootballTeam(size_t weakest_footballer_index,
                                    size_t strongest_footballer_index,
                                    const std::vector<uint64_t>& sums_of_effectivities);
uint64_t GetEffectivityFootballTeam(const std::vector<Footballer>& footballers);
std::vector<Footballer> BuildMostEffectiveSolidaryTeam(std::vector<Footballer> footballers);
std::vector<Footballer> CinFootballers();
void PrintMostEffectiveSolidaryTeam(std::vector<Footballer> footballers);
uint64_t GetEffectivityFootballTeam(const std::vector<Footballer>& footballers);

struct Footballer {
    uint64_t effectivity;
    size_t index;
    Footballer(uint64_t effectivity_constructor, uint64_t index_constructor)
        : effectivity(effectivity_constructor), index(index_constructor) {
    }
};

struct FootballTeam {
    size_t begin_index;
    size_t end_index;
    size_t effectivity;

    FootballTeam(size_t begin_index_constructor, size_t end_index_constructor,
                 size_t effectivity_constructor)
        : begin_index(begin_index_constructor),
          end_index(end_index_constructor),
          effectivity(effectivity_constructor) {
    }
};

template <class T>
class HeapSort {
private:
    size_t size_;
    typename std::vector<T>::iterator begin_;
    typename std::vector<T>::iterator end_;
    bool (*comparator_)(const T& left, const T& right);

    typename std::vector<T>::iterator GetLeftChildIterator(
        typename std::vector<T>::iterator parent_iter) const {

        return begin_ + 2 * (parent_iter - begin_) + 1;
    }

    typename std::vector<T>::iterator GetRightChildIterator(
        typename std::vector<T>::iterator parent_iter) const {

        return begin_ + 2 * (parent_iter - begin_) + 2;
    }

    void Sift(typename std::vector<T>::iterator parent_iter) {

        auto left_child_iter = GetLeftChildIterator(parent_iter);
        auto right_child_iter = GetRightChildIterator(parent_iter);

        if (left_child_iter >= end_) {  // Если нет сыновей
            return;
        }

        if (right_child_iter >= end_) {  // Есть есть левый сын, но нет правого
            if (comparator_((*parent_iter), (*left_child_iter))) {
                std::swap((*left_child_iter), (*parent_iter));
            }
            return;
        }

        if (comparator_((*right_child_iter), (*left_child_iter))) {  // Левый сын больше правого
            if (comparator_((*parent_iter), (*left_child_iter))) {
                std::swap((*left_child_iter), (*parent_iter));
                Sift(left_child_iter);
            }
        } else {  // Правый сын больше левого
            if (comparator_((*parent_iter), (*right_child_iter))) {
                std::swap((*right_child_iter), (*parent_iter));
                Sift(right_child_iter);
            }
        }
    }

    void MakeHeap() {
        for (size_t i = 1; i <= size_; ++i) {
            Sift(end_ - i);
        }
    }

    void Sort() {
        MakeHeap();

        for (size_t i = 1; i <= size_; ++i) {
            --end_;
            std::swap((*begin_), *(end_));
            Sift(begin_);
        }
    }

public:
    void operator()(typename std::vector<T>::iterator begin,
                    typename std::vector<T>::iterator end) {
        begin_ = begin;
        end_ = end;
        size_ = end - begin;
        comparator_ = [](const T& left, const T& right) { return left < right; };
        Sort();
    }

    void operator()(typename std::vector<T>::iterator begin, typename std::vector<T>::iterator end,
                    bool comparator(const T&, const T&)) {
        begin_ = begin;
        end_ = end;
        size_ = end - begin;
        comparator_ = comparator;
        Sort();
    }
};

bool ComparatorIndexFootballer(const Footballer& left, const Footballer& right) {
    return left.index < right.index;
}

bool ComparatorEffectivityFootballer(const Footballer& left, const Footballer& right) {
    return left.effectivity < right.effectivity;
}

uint64_t GetEffectivityFootballTeam(size_t weakest_footballer_index,
                                    size_t strongest_footballer_index,
                                    const std::vector<uint64_t>& sums_of_effectivities) {
    return sums_of_effectivities[strongest_footballer_index] -
           sums_of_effectivities[weakest_footballer_index];
}

std::vector<Footballer> BuildMostEffectiveSolidaryTeam(std::vector<Footballer> footballers) {

    if (footballers.size() == 1) {
        return footballers;
    }

    HeapSort<Footballer> sort;
    sort(footballers.begin(), footballers.end(), ComparatorEffectivityFootballer);

    std::vector<uint64_t> sums_of_effectivities;
    sums_of_effectivities.push_back(footballers[0].effectivity);

    for (size_t i = 1; i < footballers.size(); ++i) {
        sums_of_effectivities.push_back(sums_of_effectivities[i - 1] + footballers[i].effectivity);
    }

    FootballTeam most_effective_team(0, 1, sums_of_effectivities[1]);

    for (size_t weakest_footballer_index = 0; weakest_footballer_index < footballers.size() - 1;
         ++weakest_footballer_index) {

        size_t strongest_footballer_index =
            upper_bound(footballers.begin() + weakest_footballer_index + 2, footballers.end(),
                        Footballer(footballers[weakest_footballer_index].effectivity +
                                       footballers[weakest_footballer_index + 1].effectivity,
                                   0),
                        ComparatorEffectivityFootballer) -
            footballers.begin();

        --strongest_footballer_index;
        FootballTeam new_football_team(
            weakest_footballer_index, strongest_footballer_index,
            GetEffectivityFootballTeam(weakest_footballer_index, strongest_footballer_index,
                                       sums_of_effectivities));

        if (most_effective_team.effectivity < new_football_team.effectivity) {
            most_effective_team = new_football_team;
        }
    }

    std::vector<Footballer> result(footballers.begin() + most_effective_team.begin_index,
                                   footballers.begin() + most_effective_team.end_index + 1);
    return result;
}

uint64_t GetEffectivityFootballTeam(const std::vector<Footballer>& footballers) {
    uint64_t team_effectivity = 0;

    for (const auto& footballer : footballers) {
        team_effectivity += footballer.effectivity;
    }

    return team_effectivity;
}

std::vector<Footballer> CinFootballers() {
    size_t len_vector_of_footballers;
    std::cin >> len_vector_of_footballers;

    std::vector<Footballer> vector_of_footballers;
    uint64_t effectivity_of_footballer;

    for (size_t i = 0; i < len_vector_of_footballers; ++i) {
        std::cin >> effectivity_of_footballer;
        vector_of_footballers.emplace_back(Footballer(effectivity_of_footballer, i + 1));
    }

    return vector_of_footballers;
}

void PrintMostEffectiveSolidaryTeam(std::vector<Footballer> footballers) {
    HeapSort<Footballer> sort;
    sort(footballers.begin(), footballers.end(), ComparatorIndexFootballer);

    size_t team_effectivity = GetEffectivityFootballTeam(footballers);

    std::cout << team_effectivity << "\n";
    for (const auto& footballer : footballers) {
        std::cout << footballer.index << " ";
    }
    std::cout << "\n";
}

int main() {

    auto footballers = CinFootballers();
    auto most_effective_footballers = BuildMostEffectiveSolidaryTeam(footballers);
    PrintMostEffectiveSolidaryTeam(most_effective_footballers);

    return 0;
}