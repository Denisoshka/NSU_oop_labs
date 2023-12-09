#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include "bool_flat_map.hpp"

namespace notStd {

  const double ResizeRate_ = 1.7;// с семинаров помню что нужно использовать это число
  const size_t StartSize_ = 2;

  /*

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
      static_assert(std::default_initializable<val_type>, "ValueT must have default
    initialization"); static_assert(std::is_same<typename Allocator::value_type, value_type>::value,
                    "FlatMap must have the same value_type as its allocator");

    private:
      //  allocator_type Allocator_;
      std::unique_ptr<value_type[]> Array_;
      key_compare Comp_;
      size_type CurSize_;
      size_type MaxSize_;

      std::pair<iterator, bool> findIn_(const key_type& key) const {
        if( !CurSize_ ) {
          return {Array_.get(), false};
        }
        auto it = std::lower_bound(
                Array_.get(), Array_.get() + CurSize_, key,
                [&](const auto& otherKey, const key_type key) { return Comp_(otherKey.first, key);
    });

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
          , Comp_(Compare())
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
        Comp_ = otherMap.Comp_;
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
          value_type tmp{std::piecewise_construct, std::forward_as_tuple(key),
                         std::forward_as_tuple(std::forward<Args>(args)...)};
          *(itPair.first) = std::move(tmp);
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
  */

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
      auto it = std::lower_bound(
              Array_, Array_ + CurSize_, key,
              [&](const auto& otherKey, const key_type key) { return Comp_(otherKey.first, key); });

      return {it, it != end() && it->first == key};
    }

    std::pair<iterator, bool> prepareToInsert(const key_type& key) {
      std::pair<pointer, bool> itPair = findIn_(key);
      if( !itPair.second ) {
        if( MaxSize_ == CurSize_ ) {
          resize(static_cast<size_t>(!MaxSize_ ? StartSize_
                                               : static_cast<double>(MaxSize_) * ResizeRate_));
        }
        itPair = findIn_(key);
        for( auto toShift = end(); toShift != itPair.first; --toShift ) {
          *toShift = std::move(*(toShift - 1));
        }
      }

      return itPair;
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
      //      return tmp{otherMap};

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
      clear();
      Array_ = OtherMap.Array_;
      CurSize_ = OtherMap.CurSize_;
      MaxSize_ = OtherMap.MaxSize_;

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
      pointer tmp = std::allocator_traits<Allocator>::allocate(Allocator_, newSize);
      //      std::unique_ptr<value_type[]> tmp{std::make_unique<value_type[]>(newSize)};

      if( newSize < CurSize_ ) {
        for( auto start = Array_ + newSize; start != end(); ++start ) {
          std::allocator_traits<Allocator>::destroy(Allocator_, start);
        }
        CurSize_ = newSize;
      }
      for( auto itO = begin(), itN = tmp; itO != end(); ++itO, itN++ ) {
        alloc_traits::construct(Allocator_, itO, std::move(*(itN)));
        alloc_traits::destroy(Allocator_, itO);
        //        *itO = std::move(*itN);
      }
      //      std::move(begin(), end(), tmp);
      auto tCurSize = CurSize_;
      auto tMaxSize = MaxSize_;

      clear();
      Array_ = tmp;
      tmp = nullptr;
      CurSize_ = tCurSize;
      MaxSize_ = tMaxSize;
    }

    // доступ / вставка элемента по ключу
    ValueT& operator[](const key_type& key) {
      auto itPair = prepareToInsert(key);
      if( !itPair.second ) {
        ++CurSize_;
        //        alloc_traits::destroy(Allocator_, itPair.first);
        alloc_traits::construct(
                Allocator_, itPair.first,
                std::move(value_type{std::piecewise_construct, std::forward_as_tuple(key),
                                     std::forward_as_tuple(val_type{})}));
        //        *(itPair.first) = {key, val_type{}};
      }

      return itPair.first->second;
    }

    /*
        template<class... Args>
        std::pair<iterator, bool> try_emplace(const key_type& key, Args&&...args) {
          auto itPair = prepareToInsert(key);

          if( itPair.second = !itPair.second; itPair.second ) {
            ++CurSize_;
            value_type tmp{std::piecewise_construct, std::forward_as_tuple(key),
                           std::forward_as_tuple(std::forward<Args>(args)...)};
            *(itPair.first) = std::move(tmp);
          }

          return itPair;
        }*/

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
        alloc_traits::construct(Allocator_, it, std::move(*(it + 1)));
        alloc_traits::destroy(Allocator_, it + 1);
        //        *it = std::move(*(it + 1));
      }
      alloc_traits::destroy(Allocator_, end() - 1);
      --CurSize_;

      return 1;
    }

    // очистка таблицы, после которой maxSize_() возвращает 0, а contains() - false на любой ключ
    void clear() {
      for( auto it = begin(); it != end(); ++it ) {
        alloc_traits::destroy(Allocator_, it);
      }
      if( !Array_ ) {
        alloc_traits::deallocate(Allocator_, Array_, MaxSize_);
      }
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

  /*
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
          CurSize_ = newSize;
        }
        for( auto iTmp = tmp, iArr = Array_; iArr != Array_ + CurSize_; ++iTmp, ++iArr ) {
          alloc_traits::construct(Allocator_, iTmp, std::move(*iArr));
        }
        //      std::move(Array_, Array_ + CurSize_, tmp);
        auto size_t_tmp = CurSize_;
        clear();
        MaxSize_ = newSize;
        CurSize_ = size_t_tmp;
        Array_ = tmp;
      }

      // доступ / вставка элемента по ключу
      ValueT& operator[](const key_type& key) {
        std::pair<pointer, bool> itPair = findIn_(key);
        if( itPair.second ) {
          return itPair.first->second;
        }
        else if( CurSize_ == MaxSize_ ) {
          resize(static_cast<size_t>(!MaxSize_ ? StartSize_
                                               : static_cast<double>(MaxSize_) * ResizeRate_));
          itPair = findIn_(key);
        }

        for( auto toShift = end(); toShift != itPair.first; --toShift ) {
          *toShift = std::move(*(toShift - 1));
        }

        alloc_traits::construct(
                Allocator_, itPair.first,
                std::move(value_type{std::piecewise_construct, std::forward_as_tuple(key),
                                     std::forward_as_tuple(val_type{})}));
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
    };*/
}// namespace notStd
