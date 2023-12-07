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
         class Allocator = std::allocator<std::pair<KeyT, ValueT>>>
class FlatMap {
  using key_type = KeyT;
  using val_type = ValueT;
  using value_type = std::pair<key_type, val_type>;
  using pointer = value_type *;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using key_compare = Compare;
  using allocator_type = Allocator;
  using alloc_traits = std::allocator_traits<allocator_type>;
  using reference = value_type&;
  using const_reference = const value_type&;
  using iterator = pointer;

  static_assert(std::default_initializable<ValueT>, "ValueT must have default initialization");
  static_assert(std::is_same<typename Allocator::value_type, value_type>::value,
                "FlatMap must have the same value_type as its allocator");

private:
  allocator_type Allocator_;
  pointer Array_;
  key_compare Comp_;
  size_type CurSize_;
  size_type MaxSize_;

  std::pair<iterator, bool> findIn_(const key_type& key) const {
    if( !Array_ ) {
      return {Array_, false};
    }
    auto it = std::lower_bound(
            Array_, Array_ + CurSize_, key,
            [&](const auto otherKey, const auto key) { return Comp_(otherKey.first, key); });
    return {it, it != Array_ + CurSize_ && it->first == key};
  }

public:
  // стандартный конструктор
  FlatMap()
      : Allocator_(Allocator())
      , Array_(nullptr)
      , Comp_(Compare())
      , CurSize_(0)
      , MaxSize_(0) {
  }

  // конструктор копирования
  FlatMap(const FlatMap& otherMap)
      : FlatMap() {
    value_type *tmp = alloc_traits::allocate(Allocator_, otherMap.MaxSize_);
    try {
      std::uninitialized_copy(otherMap.begin(), otherMap.end(), tmp);
    } catch( ... ) {
      alloc_traits::deallocate(Allocator_, tmp, otherMap.MaxSize_);
      throw;
    }

    clear();
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
    clear();
  }

  // оператор присваивания
  FlatMap& operator=(const FlatMap& otherMap) {
    if( this == &otherMap ) {
      return *this;
    }

    value_type *tmp = std::allocator_traits<Allocator>::allocate(Allocator_, otherMap.MaxSize_);
    try {
      std::uninitialized_copy(otherMap.begin(), otherMap.end(), tmp);
    } catch( ... ) {
      alloc_traits::deallocate(Allocator_, tmp, otherMap.MaxSize_);
      throw;
    }

    clear();
    Array_ = tmp;
    CurSize_ = otherMap.CurSize_;
    MaxSize_ = otherMap.MaxSize_;

    return *this;
  }

  // перемещающий operator=
  FlatMap& operator=(FlatMap&& OtherMap) noexcept {
    if( this == &OtherMap ) {
      return *this;
    }
    else {
      clear();
    }

    CurSize_ = OtherMap.CurSize_;
    MaxSize_ = OtherMap.MaxSize_;
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

  void resize(size_t newSize) {
    value_type *tmp = std::allocator_traits<Allocator>::allocate(Allocator_, newSize);

    if( newSize < CurSize_ ) {
      for( auto start = Array_ + newSize; start != end(); ++start ) {
        std::allocator_traits<Allocator>::destroy(Allocator_, start);
      }
      std::move(Array_, Array_ + newSize, tmp);
    }
    else {
      std::move(Array_, Array_ + CurSize_, tmp);
    }

    clear();
    MaxSize_ = newSize;
    CurSize_ = std::min(newSize, CurSize_);
    Array_ = tmp;
  }

  // доступ / вставка элемента по ключу
  ValueT& operator[](const key_type& key) {
    if( MaxSize_ == CurSize_ ) {
      resize(static_cast<size_t>(!MaxSize_ ? StartSize_
                                           : static_cast<double>(MaxSize_) * ResizeRate_));
    }

    std::pair<pointer, bool> itPair = findIn_(key);
    if( itPair.second ) {
      return itPair.first->second;
    }

    for( auto toShift = end(); toShift != itPair.first; --toShift ) {
      alloc_traits::construct(Allocator_, std::addressof(*toShift),
                              std::move_if_noexcept(*(toShift - 1)));
    }
    key_type toKey = key;
    pointer p = itPair.first;

    //    static_assert(std::is_same<decltype(p), pointer>::value, "da chto ne tak...");
    alloc_traits::construct(Allocator_, p, std::move_if_noexcept(toKey));
    //    todo error: no matching function for call to
    //    ‘construct_at(std::pair<std::__cxx11::basic_string<char>, std::__cxx11::basic_string<char>
    //    >*&, std::__cxx11::basic_string<char>)’
    //  518 |           std::construct_at(__p, std::forward<_Args>(__args)...);
    //      |           ~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


    //    } catch( ... ) {
    //      todo fix if exception;
    //      throw ;
    //    }
    //    key_type toKey = key;
    CurSize_++;

    return itPair.first->second;
  }

  // возвращает true, если запись с таким ключом присутствует в таблице
  [[nodiscard]] bool contains(const key_type& key) const {
    return findIn_(key).second;
  }

  // удаление элемента по ключу, возвращает количество удаленных элементов (0 или 1)
  [[nodiscard]] std::size_t erase(const key_type& key) {
    auto itPair = findIn_(key);
    if( !itPair.second ) {
      return 0;
    }

    auto it = itPair.first;
    for( const auto& end = Array_ + CurSize_; it != end - 1; ++it ) {
      *it = std::move(*(it + 1));
    }
    --CurSize_;

    return 1;
  }

  // очистка таблицы, после которой maxSize_() возвращает 0, а contains() - false на любой ключ
  void clear() {
    for( auto it = begin(); it != end(); ++it ) {
      std::allocator_traits<Allocator>::destroy(Allocator_, it);
    }

    std::allocator_traits<Allocator>::deallocate(Allocator_, Array_, MaxSize_);
    Array_ = nullptr;
    CurSize_ = 0;
    MaxSize_ = 0;
  }

  // Получить итератор на первый элемент
  [[nodiscard]] iterator begin() const {
    return Array_;
  }

  // Получить итератор на элемент, следующий за последним
  [[nodiscard]] iterator end() const {
    return !Array_ ? Array_ : Array_ + CurSize_;
  }

  // Получить итератор на элемент по данному ключу, или на end(), если такого ключа нет.
  // В отличие от operator[] не создает записи для этого ключа, если её ещё нет
  [[nodiscard]] iterator find(const key_type& key) const {
    return findIn_(key).first;
  }
};
