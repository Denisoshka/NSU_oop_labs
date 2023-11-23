#pragma once
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <vector>

template<typename T, int ReservedBlocsQuantity>
struct CustomAllocator {
public:
  using value_type = T;
  using size_type = std::size_t;

  CustomAllocator()
      : StartPointer_(std::allocator_traits<std::allocator<value_type>>::allocate(
              std::allocator<value_type>(), ReservedBlocsQuantity)) {
    FreeBlocks_.reserve(ReservedBlocsQuantity);
    for( size_type i = 0; i < ReservedBlocsQuantity; ++i ) {
      FreeBlocks_.push_back(StartPointer_ + i);
    }
  };

  template<typename U>
  struct rebind {
    using other =
            CustomAllocator<U, ReservedBlocsQuantity>;// Переназначаем аллокатор для другого типа U
  };

  value_type *allocate(size_type n) {
    if( FreeBlocks_.empty() ) {
      throw std::bad_alloc();
    }
    value_type *block = FreeBlocks_.back();
    FreeBlocks_.pop_back();
    return block;
  }

  void deallocate(value_type *p, size_type n) noexcept {
    std::allocator_traits<std::allocator<T>>::deallocate(this(), p,
                                                         n);
  }

  template<class... Args>
  void construct(value_type *p, Args&&...args) {
    if constexpr( std::is_trivially_constructible_v<value_type> ) {
      std::allocator_traits<std::allocator<value_type>>::construct(this(), p,
                                                                   std::forward<Args>(args)...);
    }
  }

  void destroy(value_type *ptr) {
    if constexpr( std::is_destructible_v<value_type>) {
      std::allocator_traits<value_type>::destroy(this(), ptr);
    }
    FreeBlocks_.push_back(ptr);
  }

private:
  std::vector<value_type *> FreeBlocks_;
  size_type ReservedBlockCount_ = ReservedBlocsQuantity;
  value_type *StartPointer_;
};