#pragma once
#include <cstddef>

template <typename T>
class List {
public:
    virtual ~List() {}
    // Add element to the end.
    virtual void push_back(const T& value) = 0;
    // Insert element at position index (0..size).
    virtual void insert(size_t index, const T& value) = 0;
    // Remove element at position index if it exists.
    virtual void remove(size_t index) = 0;
    // Current number of elements.
    virtual size_t size() const = 0;
    // Remove all elements.
    virtual void clear() = 0;
};
