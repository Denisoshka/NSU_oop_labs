#pragma once

#include <concepts>
#include <iostream>
#include <map>
#include <memory>
#include <type_traits>
#include <vector>

const double ResizeRate_ = 1.7;// с семинаров помню что нужно использовать это число
const size_t StartSize_ = 2;

// std::vector<int> a;
template<class KeyT, class ValueT, class Compare = std::less<KeyT>,
         typename Allocator = std::allocator<std::pair<KeyT, ValueT>>>
class FlatMap {
  using key_type = KeyT;
  using val_type = ValueT;
  using value_type = std::pair<const KeyT, ValueT>;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using key_compare = Compare;
  using allocator_type = Allocator;
  using reference = value_type&;
  using const_reference = const value_type&;

  static_assert(std::default_initializable<ValueT>, "ValueT must have default initialization");

private:
  value_type *Array_;
  Allocator Allocator_;
  size_t CurSize_;
  size_t MaxSize_;

public:
  // стандартный конструктор
  FlatMap()
      : Allocator_()
      , Array_(nullptr)
      , CurSize_(0)
      , MaxSize_(0) {
    try {
      Array_ = std::allocator_traits<Allocator>::allocate(Allocator_, StartSize_);
      MaxSize_ = StartSize_;
    } catch( ... ) {
      Array_ = nullptr;
      throw;
    }
  }

  // конструктор копирования
  FlatMap(const FlatMap& otherMap)
      : FlatMap() {
    decltype(Array_) tmp =
            std::allocator_traits<Allocator>::allocate(Allocator_, otherMap.MaxSize_);
    try {
      std::uninitialized_copy(otherMap.begin(), otherMap.end(), tmp);
    } catch( ... ) {
      std::allocator_traits<Allocator>::deallocate(Allocator_, tmp, otherMap.MaxSize_);
      throw;
    }

    std::allocator_traits<Allocator>::deallocate(Allocator_, Array_, otherMap.MaxSize_);
    Array_ = tmp;
    CurSize_ = otherMap.CurSize_;
    MaxSize_ = otherMap.MaxSize_;
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
    for( auto& start = begin(); start != end(); start++ ) {
      std::allocator_traits<Allocator>::destroy(Allocator_, start);
    }
    std::allocator_traits<Allocator>::deallocate(Allocator_, Array_, MaxSize_);
    Array_ = nullptr;
  }

  // оператор присваивания
  FlatMap& operator=(const FlatMap& otherMap) {
    if( this == &otherMap ) {
      return *this;
    }

    std::unique_ptr<std::pair<KeyT, ValueT>[]> tmp{
            std::make_unique<std::pair<KeyT, ValueT>[]>(MaxSize_)};
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
    std::allocator_traits<Allocator>::deallocate(Allocator_, Array_, MaxSize_);
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

  void resize(size_t new_size) {
    std::unique_ptr<std::pair<KeyT, ValueT>[]> tmp =
            std::make_unique<std::pair<KeyT, ValueT>[]>(new_size);
    for( std::size_t i = 0; i < new_size; ++i ) {
      tmp[i] = std::move(Array_[i]);
    }

    MaxSize_ = new_size;
    Array_ = std::move(tmp);
  }

  // доступ / вставка элемента по ключу
  ValueT& operator[](const KeyT& key) {
    //    std::lower_bound(Array_.get(), Array_.get() + CurSize_, key) != Array_.get() + CurSize_;
    if( !MaxSize_ ) {
      Array_ = std::make_unique<std::pair<KeyT, ValueT>[]>(StartSize_);
      MaxSize_ = StartSize_;
    }

    auto it = std::lower_bound(Array_.get(), Array_.get() + CurSize_, key);
    if( it != Array_.get() + CurSize_ ) {
      return it->second;
    }

    if( MaxSize_ == CurSize_ ) {
      resize(static_cast<size_t>(static_cast<double>(MaxSize_) * ResizeRate_));
    }

    auto StartShift = it;
    for( ; it < StartShift; --StartShift ) {
      if( StartShift == Array_ ) {
        break;
      }
      *StartShift = std::move(*(StartShift - 1));
    }

    it->first = key;
    CurSize_++;

    return it->second;
  }

  // возвращает true, если запись с таким ключом присутствует в таблице
  [[nodiscard]] bool contains(const KeyT& key) const {
    return std::lower_bound(Array_.get(), Array_.get() + CurSize_, key) != Array_.get() + CurSize_;
  }

  // удаление элемента по ключу, возвращает количество удаленных элементов (0 или 1)
  [[nodiscard]] std::size_t erase(const KeyT& key) {
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

    Array_ = std::make_unique<std::pair<KeyT, ValueT>[]>(StartSize_);
    MaxSize_ = StartSize_;
  }

  // Получить итератор на первый элемент
  [[nodiscard]] std::pair<KeyT, ValueT> *begin() const {
    return Array_.get();
  }

  // Получить итератор на элемент, следующий за последним
  [[nodiscard]] std::pair<KeyT, ValueT> *end() const {
    return Array_.get() + CurSize_;
  }

  // Получить итератор на элемент по данному ключу, или на end(), если такого ключа нет.
  // В отличие от operator[] не создает записи для этого ключа, если её ещё нет
  [[nodiscard]] std::pair<KeyT, ValueT> *find(const KeyT& key) const {
    if( auto it = std::lower_bound(Array_.get(), Array_.get() + CurSize_, key);
        it != Array_.get() + CurSize_ ) {
      return it;
    }
    else {
      return Array_.get() + CurSize_;
    }
  }
};
