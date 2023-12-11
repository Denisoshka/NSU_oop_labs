#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <vector>

namespace not_alloc_flat_map {

  const double ResizeRate_ = 1.7;// с семинаров помню что нужно использовать это число
  const size_t StartSize_ = 2;

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

    //  static_assert();
    static_assert(std::is_invocable<decltype(Compare()), decltype(std::declval<KeyT>()),
                                    decltype(std::declval<KeyT>())>::value,
                  "do not support sravnenie");
    //    static_assert(std::is_invocable<bool,std::declval<Compare>(),std::declval<KeyT>(),std::declval<KeyT>()>);
    static_assert(std::default_initializable<val_type>, "ValueT must have default initialization");
    static_assert(std::is_same<typename Allocator::value_type, value_type>::value,
                  "FlatMap must have the same value_type as its allocator");

  private:
    //  allocator_type Allocator_;
    std::unique_ptr<value_type[]> Array_;
    key_compare Comp_;
    size_type CurSize_;
    size_type MaxSize_;

    std::pair<size_t, bool> findIn_(const key_type& key) const {
      if( !CurSize_ ) {
        return {CurSize_, false};
      }
      auto it = std::lower_bound(
              Array_.get(), Array_.get() + CurSize_, key,
              [&](const auto& otherKey, const key_type key) { return Comp_(otherKey.first, key); });

      return {it, it != end() && it->first == key};
    }

    std::pair<iterator, bool> prepareToInsert(const key_type& key) {
      if( MaxSize_ == CurSize_ ) {
        resize(static_cast<size_t>(!MaxSize_ ? StartSize_
                                             : static_cast<double>(MaxSize_) * ResizeRate_));
      }
      std::pair<pointer, bool> itPair = findIn_(key);
      if( !itPair.second ) {
        for( auto toShift = end(); toShift != itPair.first; --toShift ) {
          *toShift = std::move(*(toShift - 1));
        }
      }

      return itPair;
    }

  public:
    // стандартный конструктор
    FlatMap()
        : Array_(nullptr)
        , Comp_(std::declval<Compare>())
        , CurSize_(0)
        , MaxSize_(0) {
    }

    // конструктор копирования
    FlatMap(const FlatMap& otherMap)
        : FlatMap() {
      std::unique_ptr<value_type[]> tmp{std::make_unique<value_type[]>(otherMap.MaxSize_)};
      std::uninitialized_copy(otherMap.begin(), otherMap.end(), tmp.get());

      clear();
      Array_ = std::move(tmp);
      CurSize_ = otherMap.CurSize_;
      MaxSize_ = otherMap.MaxSize_;
    }

    // конструктор перемещения
    FlatMap(FlatMap&& otherMap) noexcept
        : Array_(std::move(otherMap.Array_))
        , Comp_(otherMap.Comp_)
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
      std::unique_ptr<value_type[]> tmp{std::make_unique<value_type[]>(otherMap.MaxSize_)};
      std::uninitialized_copy(otherMap.begin(), otherMap.end(), tmp.get());

      clear();
      Array_ = std::move(tmp);
      CurSize_ = otherMap.CurSize_;
      MaxSize_ = otherMap.MaxSize_;

      return *this;
    }

    // перемещающий operator=
    FlatMap& operator=(FlatMap&& OtherMap) noexcept {
      if( this == &OtherMap ) {
        return *this;
      }
      clear();
      CurSize_ = OtherMap.CurSize_;
      MaxSize_ = OtherMap.MaxSize_;
      Array_ = std::move(OtherMap.Array_);

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
      std::unique_ptr<value_type[]> tmp{std::make_unique<value_type[]>(newSize)};

      if( newSize < CurSize_ ) {
        std::move(begin(), begin() + newSize, tmp.get());
        CurSize_ = newSize;
      }
      else {
        std::move(begin(), end(), tmp.get());
      }

      //    clear();
      Array_ = std::move(tmp);
      MaxSize_ = newSize;
    }

    // доступ / вставка элемента по ключу
    ValueT& operator[](const key_type& key) {
      auto itPair = prepareToInsert(key);
      if( !itPair.second ) {
        ++CurSize_;
        *(itPair.first) = {key, val_type{}};
      }

      return itPair.first->second;
    }

    template<class... Args>
    std::pair<iterator, bool> try_emplace(const key_type& key, Args&&...args) {
      auto itPair = prepareToInsert(key);

      if( itPair.second = !itPair.second; itPair.second ) {
        ++CurSize_;
        key_type key_copy = key;
        *(itPair.first) = std::move(value_type{std::piecewise_construct,
                                               std::forward_as_tuple(std::move(key_copy)),
                                               std::forward_as_tuple(std::forward<Args>(args)...)});
      }

      return itPair;
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
      for( const auto end = this->end(); it != end - 1; ++it ) {
        *it = std::move(*(it + 1));
      }
      --CurSize_;

      return 1;
    }

    // очистка таблицы, после которой maxSize_() возвращает 0, а contains() - false на любой ключ
    void clear() {
      Array_ = nullptr;
      CurSize_ = 0;
      MaxSize_ = 0;
    }

    // Получить итератор на первый элемент
    [[nodiscard]] iterator begin() const {
      return Array_.get();
    }

    // Получить итератор на элемент, следующий за последним
    [[nodiscard]] iterator end() const {
      return !Array_ ? Array_.get() : Array_.get() + CurSize_;
    }

    // Получить итератор на элемент по данному ключу, или на end(), если такого ключа нет.
    // В отличие от operator[] не создает записи для этого ключа, если её ещё нет
    [[nodiscard]] iterator find(const key_type& key) const {
      return findIn_(key).first;
    }
  };

  template<class Compare, class Allocator>
  class FlatMap<size_t, bool, Compare, Allocator> {
    using key_type = size_t;
    using val_type = bool;
    using value_type = bool;
    using pointer = value_type *;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using key_compare = Compare;
    using allocator_type = Allocator;
    using alloc_traits = std::allocator_traits<allocator_type>;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = pointer;

    //  static_assert();
    static_assert(std::default_initializable<val_type>, "ValueT must have default initialization");
    static_assert(std::is_same<typename Allocator::value_type, value_type>::value,
                  "FlatMap must have the same value_type as its allocator");

  private:
    std::vector<value_type, Allocator> Array_;

  public:
    // стандартный конструктор
    FlatMap()
        : Array_({}) {
    }

    // конструктор копирования
    FlatMap(const FlatMap& otherMap)
        : FlatMap() {
      Array_ = otherMap.Array_;
    }

    // конструктор перемещения
    FlatMap(FlatMap&& otherMap) noexcept
        : Array_(std::move(otherMap.Array_)) {
    }

    // деструктор
    ~FlatMap() = default;

    // оператор присваивания
    FlatMap& operator=(const FlatMap& otherMap) {
      if( this == &otherMap ) {
        return *this;
      }
      Array_ = otherMap.Array_;
      return *this;
    }

    // перемещающий operator=
    FlatMap& operator=(FlatMap&& OtherMap) noexcept {
      if( this == &OtherMap ) {
        return *this;
      }
      Array_ = std::move(OtherMap.Array_);
      return *this;
    }

    // получить количество элементов в таблице
    [[nodiscard]] std::size_t size() const {
      return Array_.size();
    }

    [[noreturn]] void resize(size_t newSize) {
      Array_.resize(newSize, false);
    }

    // доступ / вставка элемента по ключу
    val_type& operator[](const key_type& key) {
      if( Array_.size() == Array_.max_size() ) {
        resize(static_cast<size_t>(
                Array_.empty() ? StartSize_ : static_cast<double>(Array_.size()) * ResizeRate_));
      }
      return Array_[key];
    }

    // возвращает true, если запись с таким ключом присутствует в таблице
    [[nodiscard]] bool contains(const key_type& key) const {
      return Array_[key];
    }

    // удаление элемента по ключу, возвращает количество удаленных элементов (0 или 1)
    [[nodiscard]] std::size_t erase(const key_type& key) {
      if( !Array_[key] ) {
        return 0;
      }
      Array_[key] = false;
      return 1;
    }

    // очистка таблицы, после которой maxSize_() возвращает 0, а contains() - false на любой ключ
    void clear() {
      Array_.clear();
    }

    // Получить итератор на первый элемент
    [[nodiscard]] iterator begin() const {
      return Array_.begin();
    }

    // Получить итератор на элемент, следующий за последним
    [[nodiscard]] iterator end() const {
      return Array_.end();
    }

    // Получить итератор на элемент по данному ключу, или на end(), если такого ключа нет.
    // В отличие от operator[] не создает записи для этого ключа, если её ещё нет
    [[nodiscard]] iterator find(const key_type& key) const {
      return Array_.begin() + key;
    }
  };
}// namespace not_alloc_flat_map

namespace alloc_flat_map {
  const double ResizeRate_ = 1.7;// с семинаров помню что нужно использовать это число
  const size_t StartSize_ = 2;

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

    static_assert(std::default_initializable<val_type>, "ValueT must have default initialization");
    static_assert(std::is_same<typename Allocator::value_type, value_type>::value,
                  "FlatMap must have the same value_type as its allocator");

  private:
    allocator_type Allocator_;
    pointer Array_;
    key_compare Comp_;
    size_type CurSize_;
    size_type MaxSize_;

    std::pair<iterator, bool> findIn_(const key_type& key) const {
      if( !CurSize_ ) {
        return {Array_, false};
      }
      auto it = std::lower_bound(begin(), end(), key, [&](const auto& otherKey, const auto& key) {
        return Comp_(otherKey.first, key);
      });

      return {it, it != end() && it->first == key};
    }

    std::pair<iterator, bool> getInsertIt_(const key_type& key) {
      auto itPair = findIn_(key);
      auto diff = itPair.first - Array_;
      if( !itPair.second ) {
        if( MaxSize_ == CurSize_ ) {
          resize(static_cast<size_t>(!MaxSize_ ? StartSize_
                                               : static_cast<double>(MaxSize_) * ResizeRate_));
        }

        for( auto toShift = end(); toShift != Array_ + diff; --toShift ) {
          alloc_traits::construct(Allocator_, std::addressof(*toShift), std::move(*(toShift - 1)));
          alloc_traits ::destroy(Allocator_, std::addressof(*(toShift - 1)));
        }
      }

      return {Array_ + diff, itPair.second};
    }

    void clearArr_() {
      if( !Array_ ) {
        return;
      }
      for( auto it = begin(); it != end(); ++it ) {
        alloc_traits::destroy(Allocator_, std::addressof(*it));
      }
      alloc_traits::deallocate(Allocator_, Array_, MaxSize_);
      Array_ = nullptr;
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
      if( otherMap.MaxSize_ ) {
        pointer tmp = alloc_traits::allocate(Allocator_, otherMap.MaxSize_);
        try {
          std::uninitialized_copy(otherMap.begin(), otherMap.end(), tmp);
        } catch( ... ) {
          alloc_traits::deallocate(Allocator_, tmp, otherMap.MaxSize_);
          throw;
        }
        clear();
        Array_ = tmp;
      }
      else {
        Array_ = nullptr;
      }
      CurSize_ = otherMap.CurSize_;
      MaxSize_ = otherMap.MaxSize_;
    }

    // конструктор перемещения
    FlatMap(FlatMap&& otherMap) noexcept
        : Array_(std::move(otherMap.Array_))
        , Comp_(otherMap.Comp_)
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
      if( otherMap.MaxSize_ ) {
        value_type *tmp = std::allocator_traits<Allocator>::allocate(Allocator_, otherMap.MaxSize_);
        try {
          std::uninitialized_copy(otherMap.begin(), otherMap.end(), tmp);
        } catch( ... ) {
          alloc_traits::deallocate(Allocator_, tmp, otherMap.MaxSize_);
          throw;
        }
        clearArr_();
        Array_ = tmp;
      }

      CurSize_ = otherMap.CurSize_;
      MaxSize_ = otherMap.MaxSize_;

      return *this;
    }

    // перемещающий operator=
    FlatMap& operator=(FlatMap&& OtherMap) noexcept {
      if( this == &OtherMap ) {
        return *this;
      }
      clearArr_();
      Array_ = OtherMap.Array_;
      CurSize_ = OtherMap.CurSize_;
      MaxSize_ = OtherMap.MaxSize_;

      OtherMap.Array_ = nullptr;
      OtherMap.CurSize_ = 0;
      OtherMap.MaxSize_ = 0;

      return *this;
    }

    // получить количество элементов в таблице
    [[nodiscard]] std::size_t size() const noexcept {
      return CurSize_;
    }

    // получить количество элементов в таблице
    [[nodiscard]] std::size_t max_size() const noexcept {
      return MaxSize_;
    }

    void reserve(size_t newSize) {
      if( newSize <= MaxSize_ ) {
        return;
      }
      pointer tmp = std::allocator_traits<Allocator>::allocate(Allocator_, newSize);
      try {
        std::uninitialized_copy(Array_, Array_ + CurSize_, tmp);
      } catch( ... ) {
        alloc_traits ::deallocate(Allocator_, tmp, newSize);
        throw;
      }

      clearArr_();
      Array_ = tmp;
      MaxSize_ = newSize;
    }

    void resize(size_t newSize) {
      pointer tmp = std::allocator_traits<Allocator>::allocate(Allocator_, newSize);
      if( newSize < CurSize_ ) {
        for( auto start = Array_ + newSize; start != end(); ++start ) {
          std::allocator_traits<Allocator>::destroy(Allocator_, start);
        }
        CurSize_ = newSize;
      }

      try {
        std::uninitialized_copy(begin(), end(), tmp);
      } catch( ... ) {
        alloc_traits ::deallocate(Allocator_, tmp, newSize);
        throw;
      }

      auto tCurSize = CurSize_;

      clear();
      Array_ = tmp;
      tmp = nullptr;
      CurSize_ = tCurSize;
      MaxSize_ = newSize;
    }

    // доступ / вставка элемента по ключу
    ValueT& operator[](const key_type& key) {
      auto itPair = getInsertIt_(key);
      if( !itPair.second ) {
        ++CurSize_;
        key_type key_copy = key;
        alloc_traits::construct(Allocator_, itPair.first,
                                std::move(value_type{std::piecewise_construct,
                                                     std::forward_as_tuple(std::move(key_copy)),
                                                     std::forward_as_tuple(val_type{})}));
      }
      return itPair.first->second;
    }

    template<class... Args>
    std::pair<iterator, bool> try_emplace(const key_type& key, Args&&...args) {
      auto itPair = getInsertIt_(key);
      if( itPair.second = !itPair.second; itPair.second ) {
        ++CurSize_;
        key_type key_copy = key;
        alloc_traits::construct(
                Allocator_, std::addressof(*itPair.first),
                std::move(value_type{std::piecewise_construct,
                                     std::forward_as_tuple(std::move(key_copy)),
                                     std::forward_as_tuple(std::forward<Args>(args)...)}));
      }

      return itPair;
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
      for( const auto end_ = end(); it != end_ - 1; ++it ) {
        alloc_traits::construct(Allocator_, std::addressof(*it), std::move(*(it + 1)));
        alloc_traits::destroy(Allocator_, std::addressof(*(it + 1)));
      }
      alloc_traits::destroy(Allocator_, std::addressof(*(end() - 1)));
      --CurSize_;

      return 1;
    }

    // очистка таблицы, после которой maxSize_() возвращает 0, а contains() - false на любой ключ
    void clear() {
      clearArr_();
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
}// namespace alloc_flat_map
