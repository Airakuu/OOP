#pragma once
#include "list.h"
#include <cstddef>
#include <utility>

// Doubly linked list similar to std::list (nodes link to prev/next).
template <typename T>
class LinkedList : public List<T> {
public:
    LinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}

    LinkedList(const LinkedList& other) : head_(nullptr), tail_(nullptr), size_(0) {
        Node* cur = other.head_;
        while (cur) {
            push_back(cur->value);
            cur = cur->next;
        }
    }

    LinkedList(LinkedList&& other) noexcept
        : head_(other.head_), tail_(other.tail_), size_(other.size_) {
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
    }

    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            LinkedList tmp(other);
            swap(tmp);
        }
        return *this;
    }

    LinkedList& operator=(LinkedList&& other) noexcept {
        if (this != &other) {
            clear();
            head_ = other.head_;
            tail_ = other.tail_;
            size_ = other.size_;
            other.head_ = nullptr;
            other.tail_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    ~LinkedList() {
        clear();
    }

    // Append element to tail.
    void push_back(const T& value) override {
        Node* node = new Node(value);
        if (!tail_) {
            head_ = tail_ = node;
        }
        else {
            node->prev = tail_;
            tail_->next = node;
            tail_ = node;
        }
        ++size_;
    }

    void push_back(T&& value) {
        Node* node = new Node(std::move(value));
        if (!tail_) {
            head_ = tail_ = node;
        }
        else {
            node->prev = tail_;
            tail_->next = node;
            tail_ = node;
        }
        ++size_;
    }

    // Insert element at index, links adjusted around it.
    void insert(size_t index, const T& value) override {
        if (index >= size_) {
            push_back(value);
            return;
        }
        if (index == 0) {
            Node* node = new Node(value);
            node->next = head_;
            if (head_) head_->prev = node;
            head_ = node;
            if (!tail_) tail_ = node;
            ++size_;
            return;
        }
        Node* cur = nodeAt(index);
        Node* node = new Node(value);
        node->prev = cur->prev;
        node->next = cur;
        cur->prev->next = node;
        cur->prev = node;
        ++size_;
    }

    // Remove node at index, fix neighbor links.
    void remove(size_t index) override {
        if (index >= size_) return;
        Node* cur = nodeAt(index);
        if (cur->prev) cur->prev->next = cur->next;
        if (cur->next) cur->next->prev = cur->prev;
        if (cur == head_) head_ = cur->next;
        if (cur == tail_) tail_ = cur->prev;
        delete cur;
        --size_;
    }

    size_t size() const override { return size_; }

    // Delete all nodes.
    void clear() override {
        Node* cur = head_;
        while (cur) {
            Node* next = cur->next;
            delete cur;
            cur = next;
        }
        head_ = nullptr;
        tail_ = nullptr;
        size_ = 0;
    }

    // Access with linked traversal.
    T& at(size_t index) {
        return nodeAt(index)->value;
    }

    const T& at(size_t index) const {
        return nodeAt(index)->value;
    }

    void swap(LinkedList& other) noexcept {
        std::swap(head_, other.head_);
        std::swap(tail_, other.tail_);
        std::swap(size_, other.size_);
    }

private:
    struct Node {
        T value;
        Node* prev;
        Node* next;
        Node(const T& v) : value(v), prev(nullptr), next(nullptr) {}
        Node(T&& v) : value(std::move(v)), prev(nullptr), next(nullptr) {}
    };

    // Walk from head/tail depending on index.
    Node* nodeAt(size_t index) const {
        if (index >= size_) return nullptr;
        if (index < size_ / 2) {
            Node* cur = head_;
            for (size_t i = 0; i < index; ++i) cur = cur->next;
            return cur;
        }
        Node* cur = tail_;
        for (size_t i = size_ - 1; i > index; --i) cur = cur->prev;
        return cur;
    }

    Node* head_;
    Node* tail_;
    size_t size_;
};
