#ifndef FLAT_MAP_LAB_FLAT_MAP_H
#define FLAT_MAP_LAB_FLAT_MAP_H

#include <iostream>

template<typename keyT, typename valueT>
class FlatMap {
private:
  const double ResizeRate_ = 1.7;// с семинаров помню что нужно использовать это число
  const size_t StartSize = 2;

  struct pair_ {
    keyT key;
    valueT value;
  };

  // возвращает индекс элемента и в случае если такой элемент существует, то 2рое значение true,
  // иначе false
  struct FoundInf {
    size_t index;
    bool IsFound;
  };

  pair_* Array_;
  size_t CurSize_;
  size_t MaxSize_;

  void resize(size_t new_size) {
    auto buf_array = new pair_[new_size];
    MaxSize_ = new_size;
    for( std::size_t i = 0; i < CurSize_; ++i ) {
      buf_array[i] = Array_[i];
    }
    delete[] Array_;
    Array_ = buf_array;
  }

  [[nodiscard]] FoundInf getIndex(const keyT& key) const {
    if( Array_[0].key == key ) {
      return {0, true};
    }
    if( CurSize_ == 0 ) {
      return {0, false};
    }
    size_t left = 0;
    size_t right = CurSize_ - 1;
    while( left <= right ) {
      size_t mid = (left + right) / 2;
      if( Array_[mid].key == key ) {
        return {mid, true};
      }
      else if( Array_[mid].key < key ) {
        left = mid + 1;
      }
      else {
        if( mid == 0 ) {
          break;
        }
        right = mid - 1;
      }
    }
    return {left, false};
  }

public:
  // стандартный конструктор
  explicit FlatMap()
      : Array_(new pair_[StartSize])
      , CurSize_(0)
      , MaxSize_(StartSize) {
  }

  // конструктор копирования
  FlatMap(const FlatMap& otherMap)
      : Array_(new pair_[otherMap.MaxSize_])
      , CurSize_(otherMap.CurSize_)
      , MaxSize_(otherMap.MaxSize_) {
    for( size_t i = 0; i < CurSize_; i++ ) {
      Array_[i] = otherMap.Array_[i];
    }
  }

  // конструктор перемещения
  FlatMap(FlatMap&& otherMap) noexcept
      : Array_(otherMap.Array_)
      , CurSize_(otherMap.CurSize_)
      , MaxSize_(otherMap.MaxSize_) {
    otherMap.CurSize_ = 0;
    otherMap.MaxSize_ = 0;
    otherMap.Array_ = nullptr;
  }

  // деструктор
  ~FlatMap() {
    delete[] Array_;
  }

  // оператор присваивания
  FlatMap& operator=(const FlatMap& otherMap) {
    if( this == &otherMap ) {
      return *this;
    }

    auto tmp = new pair_[otherMap.MaxSize_];

    delete[] Array_;
    CurSize_ = otherMap.CurSize_;
    MaxSize_ = otherMap.MaxSize_;
    Array_ = tmp;

    for( size_t i = 0; i < CurSize_; ++i ) {
      Array_[i] = otherMap.Array_[i];
    }

    return *this;
  }

  // перемещающий operator=
  FlatMap& operator=(FlatMap&& OtherMap) noexcept {
    if( this == &OtherMap ) {
      return *this;
    }
    CurSize_ = OtherMap.CurSize_;
    MaxSize_ = OtherMap.MaxSize_;
    delete[] Array_;
    Array_ = OtherMap.Array_;
    OtherMap.Array_ = nullptr;
    OtherMap.CurSize_ = 0;
    OtherMap.MaxSize_ = 0;
    return *this;
  }

  // получить количество элементов в таблице
  [[nodiscard]] std::size_t size() const {
    return CurSize_;
  }

  // доступ / вставка элемента по ключу
  valueT& operator[](const keyT& key) {
    FoundInf InsertIndex = getIndex(key);

    if( InsertIndex.IsFound ) {
      return Array_[InsertIndex.index].value;
    }

    if( MaxSize_ == CurSize_ ) {
      resize(static_cast<size_t>(static_cast<double>(MaxSize_) * ResizeRate_));
    }
    /*size_t insertIndex = 0;
    for( ; insertIndex < CurSize_ && key > Array_[insertIndex].key; insertIndex++ ) {
    }*/
    size_t StartShift = CurSize_;
    for( ; InsertIndex.index < StartShift; --StartShift ) {
      if( !StartShift ) {
        break;
      }
      Array_[StartShift] = Array_[StartShift - 1];
    }

    Array_[InsertIndex.index].key = key;
    CurSize_++;

    return Array_[InsertIndex.index].value;
  }

  // возвращает true, если запись с таким ключом присутствует в таблице
  [[nodiscard]] bool contains(const keyT& key) const {
    return getIndex(key).IsFound;
  }

  // удаление элемента по ключу, возвращает количество удаленных элементов (0 или 1)
  [[nodiscard]] std::size_t erase(const keyT& key) {
    FoundInf foundIndex = getIndex(key);
    if( !foundIndex.IsFound ) {
      return 0;
    }
    for( size_t index = foundIndex.index; index < CurSize_ - 1; ++index ) {
      Array_[index] = Array_[index + 1];
    }
    --CurSize_;
    if( CurSize_ < static_cast<size_t>(static_cast<double>(MaxSize_) / ResizeRate_) ) {
      resize(static_cast<size_t>(static_cast<double>(MaxSize_) / ResizeRate_));
    }
    return 1;
  }

  // очистка таблицы, после которой maxSize_() возвращает 0, а contains() - false на любой ключ
  void clear() {
    delete[] Array_;
    MaxSize_ = StartSize;
    CurSize_ = 0;
    Array_ = new pair_[StartSize];
  }

  /*
  class iterator {
  private:
    pair_* cur_;

  public:
    explicit iterator(pair_* i)
        : cur_(i) {
    }
  };
  */
  /*
    auto end() {
      return std::end(*Array_);
    }

    auto begin() {
      return std::begin(*Array_);
    }

    auto find(const keyT& key) {
      FoundInf keyIndex = getIndex(key);
      if(keyIndex.IsFound) {
        return std::begin(Array_ + keyIndex);
      }
      return std::end(*Array_);
    }
  */


  class iterator {
  private:
    pair_* cur_;

  public:
    explicit iterator(pair_* i)
        : cur_(i) {
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
  [[nodiscard]] iterator begin() const {
    return iterator(Array_);
  }

  // Получить итератор на элемент, следующий за последним
  [[nodiscard]] iterator end() const {
    return iterator(Array_ + CurSize_);
  }

  // Получить итератор на элемент по данному ключу, или на end(), если такого ключа нет.
  // В отличие от operator[] не создает записи для этого ключа, если её ещё нет
  [[nodiscard]] iterator find(const keyT& key) {
    FoundInf foundIndex = getIndex(key);
    if( foundIndex.IsFound ) {
      return iterator(Array_ + foundIndex.index);
    }
    return end();
  }
};

#endif// NSU_OOP_LABS_FLAT_MAP_H
