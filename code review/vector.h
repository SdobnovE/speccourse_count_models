#pragma once
#include <string.h>

template <class T>
class MyIterator : public std::iterator<std::random_access_iterator_tag, T> {
private:
    T* ptr_;
    bool direct_;

public:
    explicit MyIterator(T* ptr) : ptr_(ptr), direct_(1) {
    }

    explicit MyIterator() : ptr_(nullptr), direct_(1) {
    }
    explicit MyIterator(T* ptr, bool dir) : ptr_(ptr), direct_(dir) {
    }

    bool operator==(const MyIterator<T>& other) {
        return ptr_ == other.ptr_;
    }

    bool operator!=(const MyIterator<T>& other) {
        return !(*this == other);
    }

    const T& operator*() const {
        return *ptr_;
    }

    T& operator*() {
        return *ptr_;
    }

    T* operator->() {
        return ptr_;
    }

    MyIterator& operator++() {

        if (direct_) {
            ++ptr_;
        } else {
            --ptr_;
        }
        return *this;
    }

    MyIterator operator++(int) {
        auto res = *this;
        if (direct_) {
            ++ptr_;
        } else {
            --ptr_;
        }
        return res;
    }

    MyIterator& operator--() {
        if (direct_) {
            --ptr_;
        } else {
            ++ptr_;
        }
        return *this;
    }

    MyIterator operator--(int) {
        auto res = *this;
        if (direct_) {
            --ptr_;
        } else {
            ++ptr_;
        }
        return res;
    }

    MyIterator& operator=(const MyIterator& iter) {
        ptr_ = iter.ptr_;
        return *this;
    }

    MyIterator operator+(size_t n) const {
        return MyIterator(ptr_ + n);
    }

    MyIterator& operator+=(size_t n) {
        ptr_ += n;
        return *this;
    }

    size_t operator-(const MyIterator& other) const {
        return (static_cast<size_t>(ptr_ - other.ptr_));
    }

    MyIterator& operator-=(size_t n) {
        ptr_ -= n;
        return *this;
    }

    MyIterator operator-(size_t n) const {
        return MyIterator(ptr_ - n);
    }
};

class Vector {
private:
    int* objects_;
    int64_t size_;
    int64_t capacity_;

public:
    using Iterator = MyIterator<int>;

    Iterator begin() noexcept {  // NOLINT

        return Iterator(objects_, true);
    }

    Iterator end() noexcept {  // NOLINT

        return Iterator(objects_ + size_, true);
    }

    Iterator Begin() noexcept {

        return Iterator(objects_, true);
    }

    Iterator End() noexcept {

        return Iterator(objects_ + size_, true);
    }

    Vector(size_t size) : objects_(new int[size]), size_(size), capacity_(size) {
        for (int64_t i = 0; i < size_; ++i) {
            objects_[i] = 0;
        }
    }

    Vector() : objects_(nullptr), size_(0), capacity_(0) {
    }

    Vector(std::initializer_list<int> lis)
        : objects_(new int[(lis.end() - lis.begin())]),
          size_(lis.end() - lis.begin()),
          capacity_((lis.end() - lis.begin())) {
        auto iter = lis.begin();
        for (int64_t i = 0; i < size_; ++i) {
            objects_[i] = (*iter);
            ++iter;
        }
    }

    ~Vector() {
        delete[] objects_;
    }

    size_t Size() const {
        return size_;
    }

    size_t Capacity() const {
        return capacity_;
    }

    bool Empty() const {
        return size_ == 0;
    }

    int& operator[](size_t i) {
        return objects_[i];
    }

    void PopBack() {
        --size_;
    }

    void Clear() {
        size_ = 0;
    }

    void Resize() {
        if (size_ == 0) {
            objects_ = new int[1];
            capacity_ = 1;
            return;
        }
        int* new_memory = new int[size_ * 2];
        for (int64_t i = 0; i < size_; ++i) {
            new_memory[i] = objects_[i];
        }
        int* swapper = objects_;
        objects_ = new_memory;
        capacity_ = size_ * 2;
        delete[] swapper;
    }

    void Reserve(int64_t new_size) {
        if (new_size > capacity_) {
            int* new_memory = new int[new_size];
            for (int64_t i = 0; i < size_; ++i) {
                new_memory[i] = objects_[i];
            }
            int* swapper = objects_;
            objects_ = new_memory;
            capacity_ = new_size;
            delete[] swapper;
        }
    }

    void PushBack(int value) {
        if (size_ == capacity_) {
            Resize();
        }
        objects_[size_] = value;
        ++size_;
    }

    void Swap(Vector& right_vec) {
        int* swap_objects = right_vec.objects_;
        int64_t swap_size = right_vec.size_;
        int64_t swap_capacity = right_vec.capacity_;

        right_vec.objects_ = this->objects_;
        right_vec.capacity_ = this->capacity_;
        right_vec.size_ = this->size_;

        this->objects_ = swap_objects;
        this->capacity_ = swap_capacity;
        this->size_ = swap_size;
    }

    Vector(const Vector& vec) {
        if (vec.capacity_ == 0) {
            capacity_ = 0;
            size_ = 0;
            objects_ = nullptr;
            return;
        }
        objects_ = new int[vec.capacity_];
        size_ = vec.size_;
        capacity_ = vec.capacity_;
        for (int64_t i = 0; i < size_; ++i) {
            objects_[i] = vec.objects_[i];
        }
    }
    
    Vector& operator=(const Vector& vec) {
        if (objects_ == vec.objects_) {
            return *this;
        }
        delete[] objects_;
        if (vec.capacity_ == 0) {
            capacity_ = 0;
            objects_ = nullptr;
            size_ = 0;
            return *this;
        }
        objects_ = new int[vec.capacity_];
        size_ = vec.size_;
        capacity_ = vec.capacity_;
        for (int64_t i = 0; i < size_; ++i) {
            objects_[i] = vec.objects_[i];
        }
        return *this;
    }

};
