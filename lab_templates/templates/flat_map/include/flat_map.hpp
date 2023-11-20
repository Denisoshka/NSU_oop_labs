#pragma once

#include <iostream>
#include <memory>
#include <concepts>
#include <type_traits>

template<class keyT, class valueT, class Compare = std::less<keyT>,
         class Allocator = std::allocator<...> >
class FlatMap {
private:
  const double ResizeRate_ = 1.7;// с семинаров помню что нужно использовать это число
  const size_t StartSize_ = 2;

  struct pair_ {
    keyT key;
    valueT value;
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

public:
  // стандартный конструктор
  FlatMap()
      : Array_(nullptr)
      , CurSize_(0)
      , MaxSize_(0) {
    std::has_less<
    static_assert(std::is_);
    Array_ = nullptr;
    std::make_unique<pair_[]>(StartSize_);
    MaxSize_ = StartSize_;
  }

  // конструктор копирования
  FlatMap(const FlatMap& otherMap): FlatMap(){
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
//    std::lower_bound(Array_.get(), Array_.get() + CurSize_, key) != Array_.get() + CurSize_;
    auto it = std::lower_bound(Array_.get(), Array_.get() + CurSize_, key);
    if( it != Array_.get() + CurSize_ ) {
      return it;
    }

    if( !MaxSize_ ) {
      Array_ = std::make_unique<pair_[]>(StartSize_);
      MaxSize_ = StartSize_;
    }

    if( MaxSize_ == CurSize_ ) {
      resize(static_cast<size_t>(static_cast<double>(MaxSize_) * ResizeRate_));
    }

    auto StartShift = it;
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
    return std::lower_bound(Array_.get(), Array_.get() + CurSize_, key) != Array_.get() + CurSize_;
  }

  // удаление элемента по ключу, возвращает количество удаленных элементов (0 или 1)
  [[nodiscard]] std::size_t erase(const keyT& key) {
    auto it = std::lower_bound(Array_.get(), Array_.get() + CurSize_, key);
    if( it == (Array_.get() + CurSize_) ) {
      return 0;
    }

    for( const auto end = Array_.get() + CurSize_; it != end - 1; ++it ) {
      *it = std::move(*(it + 1));
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
  [[nodiscard]] pair_ *begin() const {
    return Array_.get();
  }

  // Получить итератор на элемент, следующий за последним
  [[nodiscard]] pair_ *end() const {
    return Array_.get() + CurSize_;
  }

  // Получить итератор на элемент по данному ключу, или на end(), если такого ключа нет.
  // В отличие от operator[] не создает записи для этого ключа, если её ещё нет
  [[nodiscard]] pair_ *find(const keyT& key) const {
    if( auto it = std::lower_bound(Array_.get(), Array_.get() + CurSize_, key);
        it != Array_.get() + CurSize_ ) {
      return it;
    }
    else {
      return Array_.get() + CurSize_;
    }
  }
};
