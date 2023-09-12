#ifndef FLAT_MAP_LAB_FLAT_MAP_H
#define FLAT_MAP_LAB_FLAT_MAP_H

#include <iostream>

template<typename keyT, typename valueT>
class FlatMap {
  private:
  const double resizeRate_ = 1.7;// с семинаров помню что нужно использовать это число
  const size_t startSize = 2;

  struct pair_ {
    keyT key;
    valueT value;
  };
  // возвращает индекс элемента и в случае если такой элемент существует, то 2рое значение true,
  // иначе false
  struct foundInf {
    size_t index;
    bool isFound;
  };

  pair_* array_;
  size_t curSize_;
  size_t maxSize_;

  void resize(size_t new_size) {
    auto* buf_array = new pair_[new_size];
    this->maxSize_ = new_size;
    for (std::size_t i = 0; i < this->curSize_; ++i) {
      buf_array[i] = this->array_[i];
    }
    delete[] array_;
    this->array_ = buf_array;
  }
  [[nodiscard]] foundInf getIndex(const keyT& key) const {
    if (array_[0].key == key) {
      return {0, true};
    }
    size_t left = 1;
    size_t right = (curSize_) ? curSize_ - 1 : curSize_;
    while (left <= right) {
      size_t mid = (left + right) / 2;
      if (array_[mid].key == key) {
        return {mid, true};
      }
      else if (array_[mid].key < key) {
        left = mid + 1;
      }
      else {
        right = mid - 1;
      }
    }
    return {0, false};
  }

  public:
  // стандартный конструктор
  explicit FlatMap() : array_(new pair_[startSize]), curSize_(0), maxSize_(startSize) {
  }

  // конструктор копирования
  FlatMap(const FlatMap& otherMap)
      : curSize_(otherMap.curSize_)
      , maxSize_(otherMap.maxSize_)
      , array_(new pair_[maxSize_]) {
    for (size_t i = 0; i < curSize_; i++) {
      array_[i] = otherMap.array_[i];
    }
  }

  // конструктор перемещения
  FlatMap(FlatMap&& otherMap) noexcept
      : array_(otherMap.array_)
      , curSize_(otherMap.curSize_)
      , maxSize_(otherMap.maxSize_) {
    otherMap.curSize_ = 0;
    otherMap.maxSize_ = 0;
    otherMap.array_ = nullptr;
  }

  // деструктор
  ~FlatMap() {
    delete[] array_;
  }

  // оператор присваивания
  FlatMap& operator=(const FlatMap& otherMap) {
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

  // перемещающий operator=
  FlatMap& operator=(FlatMap&& otherMap) noexcept {
    if (this == &otherMap) {
      return *this;
    }
    curSize_ = otherMap.curSize_;
    maxSize_ = otherMap.maxSize_;
    delete[] array_;
    array_ = otherMap.array_;
    otherMap.array_ = nullptr;
    otherMap.curSize_ = 0;
    otherMap.maxSize_ = 0;
    return *this;
  }

  // получить количество элементов в таблице
  [[nodiscard]] std::size_t size() const {
    return curSize_;
  }

  // доступ / вставка элемента по ключу
  valueT& operator[](const keyT& key) {
    foundInf indexKey = getIndex(key);

    if (indexKey.isFound) {
      return array_[indexKey.index].value;
    }

    if (maxSize_ == curSize_) {
      resize(static_cast<size_t>(static_cast<double>(this->maxSize_) * this->resizeRate_));
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

    this->array_[insertIndex].key = key;
    this->curSize_++;

    return this->array_[insertIndex].value;
  }

  // возвращает true, если запись с таким ключом присутствует в таблице
  [[nodiscard]] bool contains(const keyT& key) const {
    return getIndex(key).isFound;
  }

  // удаление элемента по ключу, возвращает количество удаленных элементов (0 или 1)
  [[nodiscard]] std::size_t erase(const keyT& key) {
    foundInf foundIndex = getIndex(key);
    if (!foundIndex.isFound) {
      return 0;
    }
    for (size_t index = foundIndex.index; index < curSize_ - 1; ++index) {
      array_[index] = array_[index + 1];
    }
    --curSize_;
    if (curSize_ < static_cast<size_t>(static_cast<double>(maxSize_) / resizeRate_)) {
      resize(static_cast<size_t>(static_cast<double>(maxSize_) / resizeRate_));
    }
    return 1;
  }

  // очистка таблицы, после которой maxSize_() возвращает 0, а contains() - false на любой ключ
  void clear() {
    delete[] array_;
    maxSize_ = startSize;
    curSize_ = 0;
    array_ = new pair_[startSize];
  }


  class iterator {
private:
    pair_* cur_;

public:
    const keyT& first = cur_->key;
    valueT& second = cur_->value;

    explicit iterator(pair_* i) : cur_(i) {
    }

    bool operator==(const iterator& other_it) const {
      return cur_ == other_it.cur_;
    }

    bool operator!=(const iterator& other_it) const {
      return cur_ != other_it.cur_;
    }

    iterator& operator++() {
      ++cur_;
      return *this;
    };

    iterator operator++(int) {
      iterator tmp = *this;
      ++cur_;
      return tmp;
    };
  };

  // Получить итератор на первый элемент
  [[nodiscard]] iterator begin() {
    return iterator(array_);
  }

  // Получить итератор на элемент, следующий за последним
  [[nodiscard]] iterator end() {
    return iterator(array_ + curSize_);
  }

  // Получить итератор на элемент по данному ключу, или на end(), если такого ключа нет.
  // В отличие от operator[] не создает записи для этого ключа, если её ещё нет

  [[nodiscard]] iterator find(const keyT& key) {
    foundInf foundIndex = getIndex(key);
    if (foundIndex.isFound) {
      return iterator(array_ + foundIndex.index);
    }
    return end();
  }
};

#endif// NSU_OOP_LABS_FLAT_MAP_H
