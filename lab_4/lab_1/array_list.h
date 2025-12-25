#pragma once
#include "list.h"
#include <cstddef>
#include <new>
#include <utility>

// Array-based list similar to std::vector (dynamic array + growth).
template <typename T>
class ArrayList : public List<T> {
public:
    ArrayList() : data_(nullptr), size_(0), capacity_(0) {}

    ArrayList(const ArrayList& other) : data_(nullptr), size_(0), capacity_(0) {
        reserve(other.size_);
        for (size_t i = 0; i < other.size_; ++i) {
            new (data_ + i) T(other.data_[i]);
        }
        size_ = other.size_;
    }

    ArrayList(ArrayList&& other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    ArrayList& operator=(const ArrayList& other) {
        if (this != &other) {
            ArrayList tmp(other);
            swap(tmp);
        }
        return *this;
    }

    ArrayList& operator=(ArrayList&& other) noexcept {
        if (this != &other) {
            clear();
            ::operator delete(data_);
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    ~ArrayList() {
        clear();
        ::operator delete(data_);
    }

    // Ensure capacity for at least newCapacity elements.
    void reserve(size_t newCapacity) {
        if (newCapacity <= capacity_) return;
        T* newData = static_cast<T*>(::operator new(sizeof(T) * newCapacity));
        for (size_t i = 0; i < size_; ++i) {
            new (newData + i) T(std::move(data_[i]));
            data_[i].~T();
        }
        ::operator delete(data_);
        data_ = newData;
        capacity_ = newCapacity;
    }

    // Destroy all elements, keep allocated buffer.
    void clear() override {
        for (size_t i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        size_ = 0;
    }

    size_t size() const override { return size_; }

    // Append element by copy.
    void push_back(const T& value) override {
        if (size_ == capacity_) reserve(capacity_ ? capacity_ * 2 : 4);
        new (data_ + size_) T(value);
        ++size_;
    }

    // Append element by move.
    void push_back(T&& value) {
        if (size_ == capacity_) reserve(capacity_ ? capacity_ * 2 : 4);
        new (data_ + size_) T(std::move(value));
        ++size_;
    }

    // Insert element, shifting tail to the right.
    void insert(size_t index, const T& value) override {
        if (index > size_) index = size_;
        if (size_ == capacity_) reserve(capacity_ ? capacity_ * 2 : 4);
        for (size_t i = size_; i > index; --i) {
            new (data_ + i) T(std::move(data_[i - 1]));
            data_[i - 1].~T();
        }
        new (data_ + index) T(value);
        ++size_;
    }

    // Remove element and shift tail to the left.
    void remove(size_t index) override {
        if (index >= size_) return;
        data_[index].~T();
        for (size_t i = index; i + 1 < size_; ++i) {
            new (data_ + i) T(std::move(data_[i + 1]));
            data_[i + 1].~T();
        }
        --size_;
    }

    // Direct access without bounds checks.
    T& operator[](size_t index) { return data_[index]; }
    const T& operator[](size_t index) const { return data_[index]; }

    T* begin() { return data_; }
    T* end() { return data_ + size_; }
    const T* begin() const { return data_; }
    const T* end() const { return data_ + size_; }

    void swap(ArrayList& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

private:
    T* data_;
    size_t size_;
    size_t capacity_;
};
