#include "flat_map.h"

const double resizeRate_ = 1.7;// с семинаров помню что нужно использовать это число
const size_t startSize = 2;

[[nodiscard]] size_t FlatMap::getIndex(const std::string &key) const {
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

FlatMap::FlatMap(FlatMap &&otherMap) noexcept
    : array_(otherMap.array_)
    , curSize_(otherMap.curSize_)
    , maxSize_(otherMap.maxSize_) {
  otherMap.array_ = nullptr;
}


FlatMap::~FlatMap() {
  delete[] array_;
}

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


FlatMap &FlatMap::operator=(FlatMap &&otherMap) noexcept {
  if (this == &otherMap) {
    return *this;
  }
  curSize_ = otherMap.curSize_;
  maxSize_ = otherMap.maxSize_;
  delete[] array_;
  array_ = otherMap.array_;
  otherMap.array_ = nullptr;
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
  for (size_t index = getIndex(key); index < curSize_ - 1; ++index) {
    array_[index] = array_[index + 1];
  }
<<<<<<< Updated upstream
  --curSize_;
  if (curSize_ < (size_t) ((double) maxSize_ / resizeRate_)) {
    resize(size_t((double) maxSize_ / resizeRate_));
=======
  if (curSize_ < static_cast<size_t>(static_cast<double>(maxSize_) / resizeRate_)) {
    resize(static_cast<size_t>(static_cast<double>(maxSize_) / resizeRate_));
>>>>>>> Stashed changes
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
    resize(static_cast<size_t>(static_cast<double>(maxSize_) * resizeRate_));
  }
  size_t insertIndex = 0;
  for (; insertIndex < curSize_ && key > array_[insertIndex].key; insertIndex++) {
  }
  size_t startShift = curSize_;
  for (; insertIndex < startShift; --startShift) {
    if (!startShift) {
      break;
    }
    array_[startShift] = array_[startShift - 1];
  }

  array_[insertIndex].key = key;
  curSize_++;

  return array_[insertIndex].value;
}

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


FlatMap::iterator::iterator(pair_ *ptr) : cur_(ptr) {
}

bool FlatMap::iterator::operator==(const iterator &other_it) const {
  return cur_ == other_it.cur_;
}


bool FlatMap::iterator::operator!=(const iterator &other_it) const {
  return cur_ != other_it.cur_;
}

FlatMap::iterator &FlatMap::iterator::operator++() {
  ++cur_;
  return *this;
}


[[nodiscard]] FlatMap::iterator FlatMap::begin() {
  return iterator(array_);
}

[[nodiscard]] FlatMap::iterator FlatMap::end() {
  return iterator(array_ + curSize_);
}

FlatMap::iterator FlatMap::find(const std::string &key) {
  if (contains(key)) {
    return iterator(array_ + getIndex(key));
  }
  return FlatMap::end();
}
