#pragma once
#include <iostream>
#include <memory>

template <typename T, typename Allocator = std::allocator<T>> class CustomList {

  struct Node {
    explicit Node(const T &data) : data_(data), next_(nullptr) {}
    explicit Node(T&& data): data_(std::move(data)), next_(nullptr) {}

    T data_;
    Node *next_;
  };

   using AllocatorType = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;

public:
  CustomList() : head_(nullptr), tail_(nullptr), size_(0) {}
  ~CustomList() { clear(); }

  void push_back(const T &value) {
    Node *node = alloc_.allocate(1);
    if (!head_) {
      head_ = tail_ = node;
    } else {
      tail_->next_ = node;
      tail_ = node;
    }
    node->data_ = value;
    ++size_;
  }

  void clear() {
    Node *current = head_;
    while (current) {
      Node *next = current->next_;
      alloc_.deallocate(current, 1);
      current = next;
    }
    head_ = tail_ = nullptr;
    size_ = 0;
  }

  Node *head() { return head_; }

  std::size_t size() const { return size_; }
  bool empty() const { return size_ == 0; }

private:
  Node *head_;
  Node *tail_;
  size_t size_;
  AllocatorType alloc_;
};