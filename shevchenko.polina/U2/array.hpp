#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <cstddef>

namespace shevchenko {

template <typename T>
class Array {
private:
  T* data_;
  size_t size_;
  size_t capacity_;

  void resize(size_t new_capacity) {
    T* new_data = new T[new_capacity];
    for (size_t i = 0; i < size_; ++i) {
      new_data[i] = data_[i];
    }
    delete[] data_;
    data_ = new_data;
    capacity_ = new_capacity;
  }

public:
  Array()
  : data_(nullptr),
  size_(0),
  capacity_(0) {}

  ~Array() {
    delete[] data_;
  }

  void push_back(const T& value) {
    if (size_ >= capacity_) {
      size_t new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
      resize(new_capacity);
    }
    data_[size_] = value;
    ++size_;
  }

  void pop_back() {
    if (size_ > 0) {
      --size_;
    }
  }

  T& operator[](size_t index) {
    return data_[index];
  }

  const T& operator[](size_t index) const {
    return data_[index];
  }

  size_t size() const {
    return size_;
  }

  bool empty() const {
    return size_ == 0;
  }

  void clear() {
    delete[] data_;
    data_ = nullptr;
    size_ = 0;
    capacity_ = 0;
  }

  T* begin() {
    return data_;
  }

  const T* begin() const {
    return data_;
  }

  T* end() {
    return data_ + size_;
  }

  const T* end() const {
    return data_ + size_;
  }
};

}

#endif
