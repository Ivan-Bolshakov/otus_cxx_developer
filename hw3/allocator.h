#pragma once

#include <array>
#include <memory>

template <typename T, size_t BUF_SIZE> class CustomAllocator {
public:
  using value_type = T;
  using pointer = T *;
  using const_pointer = const T *;
  using reference = T &;
  using const_reference = const T &;
  using size_type = std::size_t;
  using difference_type =
      typename std::pointer_traits<pointer>::difference_type;
  CustomAllocator() {
    buffer_ = reinterpret_cast<T *>(std::malloc(BUF_SIZE * sizeof(T)));
    if (!buffer_) {
      std::cerr << "Allocate memory error\n";
      throw std::bad_alloc();
    }
  }

  ~CustomAllocator() {
    if (buffer_) {
      std::free(buffer_);
    }
  }

  template <typename U> struct rebind {
    using other = CustomAllocator<U, BUF_SIZE>;
  };

  T *allocate(size_t n) {
    if (n == 0) {
      return nullptr;
    }
    T *cur_ptr = GetFreeBlock(n);
    if (!cur_ptr) {
      std::cerr << "CustomAllocator failed to find available memory! capacity: "
                << BUF_SIZE << " current size: " << size_
                << " requested size: " << n << "\n";
      throw std::bad_alloc();
    }
    return cur_ptr;
  }

  void deallocate(T *ptr, size_t n) { Free(ptr, n); }

private:
  T *GetFreeBlock(size_t n) {
    if (size_ + n > BUF_SIZE) {
      std::cerr << "Custom Allocaor buffer is full\n";
      throw std::bad_alloc();
    }
    size_t index = 0;
    size_t counter = 0;
    for (size_t i = 0; i < BUF_SIZE; ++i) {
      if (used_[i]) {
        index = i + 1;
        counter = 0;
        continue;
      } else {
        ++counter;
      }
      if (counter == n) {
        for (size_t i = 0; i < n; ++i) {
          used_[index + i] = true;
        }
        size_ += n;
        return &buffer_[index];
      }
    }
    return nullptr;
  }

  void Free(T *ptr, size_t n) {
    if (ptr < buffer_ || ptr > buffer_ + BUF_SIZE - 1) {
      std::cerr << "CustomAllocator failed to free for invalid pointer\n";
      throw std::bad_alloc();
    }
    size_t index = ptr - buffer_;
    size_t index_end = index + n;
    if (index_end > BUF_SIZE) {
      std::cerr << "custom_allocator trying to occupy memory out of range!\n";
      throw std::out_of_range(
          "custom_allocator trying to occupy memory out of range!");
    }
    for (; index < index_end; ++index) {
      if (used_[index]) {
        used_[index] = false;
        size_--;
      }
    }
  }

private:
  T *buffer_{nullptr};
  bool used_[BUF_SIZE] = {};
  size_t size_{0};
};