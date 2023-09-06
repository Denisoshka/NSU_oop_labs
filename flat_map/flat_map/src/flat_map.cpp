#include "flat_map.h"

size_t FlatMap::getIndex(const std::string &key) const {
  size_t left = 1, right = curSize_ - 1;
  while (left <= right) {
    size_t mid = (left + right) / 2;
    if (array_[mid].key == key) {
      return mid;
    }
    else if (array_[mid].key < key) {
      left = mid + 1;
    }
    else {
      right = mid - 1;
    }
  }
  return 0;
}

FlatMap::FlatMap() : array_(new pair_[startSize]), curSize_(0), maxSize_(startSize) {
}

FlatMap::FlatMap(const FlatMap &otherMap) {
  curSize_ = otherMap.curSize_;
  maxSize_ = otherMap.maxSize_;
  array_ = new pair_[maxSize_];
  for (size_t i = 0; i < curSize_; i++) {
    array_[i] = otherMap.array_[i];
  }
}

FlatMap::~FlatMap() {
  delete[] array_;
};

FlatMap &FlatMap::operator=(const FlatMap &otherMap) {
  if (this == &otherMap) {
    return *this;
  }
  delete[] array_;

  curSize_ = otherMap.curSize_;
  maxSize_ = otherMap.maxSize_;
  array_ = new pair_[otherMap.maxSize_];

  for (size_t i = 0; i < curSize_; ++i) {
    array_[i] = otherMap.array_[i];
  }
  return *this;
}

[[nodiscard]] bool FlatMap::contains(const std::string &key) const {
  if (curSize_ == 0) {
    return false;
  }
  if (array_[0].key == key) {
    return true;
  }

  if (getIndex(key) == 0) {
    return false;
  }
  return true;
}

[[nodiscard]] std::size_t FlatMap::erase(const std::string &key) {
  if (!contains(key)) {
    return 0;
  }
  for (size_t index = getIndex(key); index < curSize_ - 1; index++) {
    array_[index] = array_[index + 1];
  }
  if (curSize_ < (size_t) ((double) maxSize_ / resizeRate_)) {
    resize(size_t((double) maxSize_ / resizeRate_));
  }
  return 1;
}

[[nodiscard]] std::size_t FlatMap::size() const {
  return curSize_;
};

std::string &FlatMap::operator[](const std::string &key) {
  if (contains(key)) {
    size_t index_key = getIndex(key);
    return array_[index_key].value;
  }

  if (maxSize_ == curSize_) {
    resize(size_t((double) maxSize_ * resizeRate_));
  }
  size_t insertIndex = 0;
  for (; insertIndex < curSize_ && array_[insertIndex].key < key; insertIndex++) {
  }

  for (size_t shiftIndex = insertIndex; shiftIndex < curSize_;
       array_[shiftIndex].key = array_[shiftIndex + 1].key, shiftIndex++) {
  }

  array_[insertIndex].key = key;
  array_[insertIndex].value = "...";
  curSize_++;

  return array_[insertIndex].value;
};

void FlatMap::resize(const size_t new_size) {
  auto *buf_array = new pair_[new_size];
  maxSize_ = new_size;
  for (std::size_t i = 0; i < curSize_; ++i) {
    buf_array[i] = array_[i];
  }
  delete[] array_;
  array_ = buf_array;
}

void FlatMap::clear() {
  delete[] array_;
  maxSize_ = startSize;
  curSize_ = 0;
  array_ = new pair_[startSize];
}
