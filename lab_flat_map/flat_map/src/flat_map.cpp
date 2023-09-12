#include "flat_map.h"

/*
template<typename keyT, typename valueT>

}*/
//template<typename keyT, typename valueT>

/*template<typename keyT, typename valueT>
FlatMap<keyT, valueT>::FlatMap(const FlatMap &otherMap) {
  curSize_ = otherMap.curSize_;
  maxSize_ = otherMap.maxSize_;
  array_ = new pair_[maxSize_];
  for (size_t i = 0; i < curSize_; i++) {
    array_[i] = otherMap.array_[i];
  }
}*/
/*
template<typename keyT, typename valueT>
FlatMap<keyT, valueT>::FlatMap(FlatMap &&otherMap) noexcept
    : array_(otherMap.array_)
    , curSize_(otherMap.curSize_)
    , maxSize_(otherMap.maxSize_) {
  otherMap.array_ = nullptr;
}*/
/*

template<typename keyT, typename valueT>
FlatMap<keyT, valueT>::~FlatMap() {
  delete[] array_;
}
*/

//template<typename keyT, typename valueT>
/*
FlatMap<keyT, valueT> &FlatMap<keyT, valueT>::operator=(const FlatMap &otherMap) {
  if (this == &otherMap) {
    return *this;
  }
  delete[] array_;

  this->curSize_ = otherMap.curSize_;
  this->maxSize_ = otherMap.maxSize_;
  this->array_ = new pair_[otherMap.maxSize_];

  for (size_t i = 0; i < this->curSize_; ++i) {
    this->array_[i] = otherMap.array_[i];
  }

  return *this;
}
*/
/*
template<typename keyT, typename valueT>
[[nodiscard]] bool FlatMap<keyT, valueT>::contains(const keyT &key) const {
  if (this->curSize_ == 0) {
    return false;
  }
  if (array_[0].key == key) {
    return true;
  }

  if (getIndex<keyT, valueT>(key) == 0) {
    return false;
  }
  return true;
}
*/
/*
template<typename keyT, typename valueT>
[[nodiscard]] std::size_t FlatMap<keyT, valueT>::erase(const keyT &key) {
  if (!contains<keyT>(key)) {
    return 0;
  }
  for (size_t index = getIndex(key); index < curSize_ - 1; ++index) {
    array_[index] = array_[index + 1];
  }
  --curSize_;
  if (curSize_ < static_cast<size_t>(static_cast<double>(maxSize_) / resizeRate_)) {
    resize(static_cast<size_t>(static_cast<double>(maxSize_) / resizeRate_));
  }
  return 1;
}

template<typename keyT, typename valueT>
[[nodiscard]] std::size_t FlatMap<keyT, valueT>::size() const {
  return curSize_;
}*/


/*
template<typename keyT, typename valueT>
FlatMap<keyT, valueT> &FlatMap<keyT, valueT>::operator=(FlatMap &&otherMap) noexcept {
  if (this == &otherMap) {
    return *this;
  }
  this->curSize_ = otherMap.curSize_;
  this->maxSize_ = otherMap.maxSize_;
  delete[] this->array_;
  this->array_ = otherMap.array_;
  otherMap.array_ = nullptr;
  return *this;
}*/
/*

template<class keyT, class valueT>
valueT &FlatMap<keyT, valueT>::operator[](const keyT &key) {
  if (contains<keyT>(key)) {
    size_t index_key = getIndex(key);
    return array_[index_key].value;
  }

  if (this->maxSize_ == this->curSize_) {
    resize(static_cast<size_t>(static_cast<double>(this->maxSize_) * this->resizeRate_));
  }
  size_t insertIndex = 0;
  for (; insertIndex < this->curSize_ && key > array_[insertIndex].key; insertIndex++) {
  }
  size_t startShift = this->curSize_;
  for (; insertIndex < startShift; --startShift) {
    if (!startShift) {
      break;
    }
    this->array_[startShift] = this->array_[startShift - 1];
  }

  this->array_[insertIndex].key = key;
  this->curSize_++;

  return this->array_[insertIndex].value;
}
*/

/*template<class keyT, class valueT>
void FlatMap<keyT, valueT>::resize(const size_t new_size) {
  auto *buf_array = new pair_[new_size];
  this->maxSize_ = new_size;
  for (std::size_t i = 0; i < this->curSize_; ++i) {
    buf_array[i] = this->array_[i];
  }
  delete[] array_;
  this->array_ = buf_array;
}*/
/*

template<class keyT, class valueT>
void FlatMap<keyT, valueT>::clear() {
  delete[] array_;
  this->maxSize_ = startSize;
  this->curSize_ = 0;
  this->array_ = new pair_[startSize];
}
*/

/*
template<class keyT, class valueT>
FlatMap<keyT, valueT>::iterator::iterator(pair_ *ptr) : cur_(ptr) {
}

template<class keyT, class valueT>
bool FlatMap<keyT, valueT>::iterator::operator==(const iterator &other_it) const {
  return cur_ == other_it.cur_;
}
*/

/*
template<class keyT, class valueT>
bool FlatMap<keyT, valueT>::iterator::operator!=(const iterator &other_it) const {
  return cur_ != other_it.cur_ ;
}
*/

/*

template<class keyT, class valueT>
FlatMap<keyT, valueT>::iterator &FlatMap<keyT, valueT>::iterator::operator++() {
  ++cur_;
  return *this;
}

template<class keyT, class valueT>
[[nodiscard]] FlatMap<keyT, valueT>::iterator FlatMap<keyT, valueT>::begin() {
  return iterator(array_);
}

template<class keyT, class valueT>
[[nodiscard]] FlatMap<keyT, valueT>::iterator FlatMap<keyT, valueT>::end() {
  return iterator(array_ + curSize_);
}

template<class keyT, class valueT>
FlatMap<keyT, valueT>::iterator FlatMap<keyT, valueT>::find(const keyT &key) {
  if (contains(key)) {
    return iterator(array_ + getIndex(key));
  }
  return FlatMap::end();
}
*/
