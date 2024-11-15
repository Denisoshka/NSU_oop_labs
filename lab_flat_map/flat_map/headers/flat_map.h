#ifndef FLAT_MAP_FLAT_MAP_H
#define FLAT_MAP_FLAT_MAP_H

#include <iostream>
#include <memory>

template<typename keyT, typename valueT>
class FlatMap {
private:
  const double ResizeRate_ = 1.7;// с семинаров помню что нужно использовать это число
  const size_t StartSize_ = 2;

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

  std::unique_ptr<pair_[]> Array_;
  size_t CurSize_;
  size_t MaxSize_;

  void resize(size_t new_size) {
    std::unique_ptr<pair_[]> tmp = std::make_unique<pair_[]>(new_size);
    for( std::size_t i = 0; i < CurSize_; ++i ) {
      tmp[i] = Array_[i];
    }

    MaxSize_ = new_size;
    Array_ = std::move(tmp);
  }

  [[nodiscard]] FoundInf getIndex(const keyT& key) const {
    if( CurSize_ == 0 ) {
      return {0, false};
    }
    if( Array_[0].key == key ) {
      return {0, true};
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
  FlatMap()
      : Array_(nullptr)
      , CurSize_(0)
      , MaxSize_(0) {
    Array_ = std::make_unique<pair_[]>(StartSize_);
    MaxSize_ = StartSize_;
  }

  // конструктор копирования
  FlatMap(const FlatMap& otherMap)
      : Array_(nullptr)
      , CurSize_(0)
      , MaxSize_(0) {
    std::unique_ptr<pair_[]> tmp = std::make_unique<pair_[]>(otherMap.MaxSize_);
    for( size_t i = 0; i < CurSize_; ++i ) {
      tmp[i] = otherMap.Array_[i];
    }

    Array_ = std::move(tmp);
    CurSize_ = otherMap.CurSize_;
    MaxSize_ = otherMap.MaxSize_;
  }

  // конструктор перемещения
  FlatMap(FlatMap&& otherMap) noexcept
      : Array_(std::move(otherMap.Array_))
      , CurSize_(otherMap.CurSize_)
      , MaxSize_(otherMap.MaxSize_) {
    otherMap.CurSize_ = 0;
    otherMap.MaxSize_ = 0;
    otherMap.Array_ = nullptr;
  }

  // деструктор
  ~FlatMap() {
    Array_.reset();
  }

  // оператор присваивания
  FlatMap& operator=(const FlatMap& otherMap) {
    if( this == &otherMap ) {
      return *this;
    }

    std::unique_ptr<pair_[]> tmp{std::make_unique<pair_[]>(MaxSize_)};
    for( size_t i = 0; i < CurSize_; ++i ) {
      tmp[i] = otherMap.Array_[i];
    }

    CurSize_ = otherMap.CurSize_;
    MaxSize_ = otherMap.MaxSize_;
    Array_ = std::move(tmp);

    return *this;
  }

  // перемещающий operator=
  FlatMap& operator=(FlatMap&& OtherMap) noexcept {
    if( this == &OtherMap ) {
      return *this;
    }
    CurSize_ = OtherMap.CurSize_;
    MaxSize_ = OtherMap.MaxSize_;
    Array_.reset();
    Array_ = std::move(OtherMap.Array_);
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

    if( !MaxSize_ ) {
      Array_ = std::make_unique<pair_[]>(StartSize_);
      MaxSize_ = StartSize_;
    }

    if( MaxSize_ == CurSize_ ) {
      resize(static_cast<size_t>(static_cast<double>(MaxSize_) * ResizeRate_));
    }

    size_t StartShift = CurSize_;
    for( ; InsertIndex.index < StartShift; --StartShift ) {
      if( !StartShift ) {
        break;
      }
      Array_[StartShift] = std::move(Array_[StartShift - 1]);
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
      Array_[index] = std::move(Array_[index + 1]);
    }
    --CurSize_;

    return 1;
  }

  // очистка таблицы, после которой maxSize_() возвращает 0, а contains() - false на любой ключ
  void clear() {
    Array_.reset();
    MaxSize_ = 0;
    CurSize_ = 0;

    Array_ = std::make_unique<pair_[]>(StartSize_);
    MaxSize_ = StartSize_;
  }

  // Получить итератор на первый элемент
  [[nodiscard]] pair_* begin() const {
    return Array_.get();
  }

  // Получить итератор на элемент, следующий за последним
  [[nodiscard]] pair_* end() const {
    return Array_.get() + CurSize_;
  }

  // Получить итератор на элемент по данному ключу, или на end(), если такого ключа нет.
  // В отличие от operator[] не создает записи для этого ключа, если её ещё нет
  [[nodiscard]] pair_* find(const keyT& key) const {
    FoundInf inf = getIndex(key);
    if( inf.IsFound ) {
      return Array_.get() + inf.index;
    }
    return Array_.get() + CurSize_;
  }
};

#endif// FLAT_MAP_FLAT_MAP_H
