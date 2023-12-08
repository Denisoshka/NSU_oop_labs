#pragma once
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <vector>

template<class T, size_t ReservedBlocsQuantity = 1>
struct CustomAllocator {
  static_assert(!std::is_same_v<T, void>, "Type of the allocator can not be void");
  static_assert(ReservedBlocsQuantity >= 1, "ReservedBlocsQuantity must be greater that zero");

public:
  template<typename U>
  struct rebind {
    using other = CustomAllocator<U, ReservedBlocsQuantity>;
  };

  using value_type = T;
  using pointer = value_type *;
  using size_type = std::size_t;
  using allocator_type = std::allocator<value_type>;
  using alloc_traits = std::allocator_traits<allocator_type>;

private:
  allocator_type Allocator_;
  pointer Memory_;
  std::vector<pointer> FreeBlocks_;

public:
  CustomAllocator()
      : Allocator_(allocator_type())
      , Memory_(alloc_traits::allocate(Allocator_, ReservedBlocsQuantity)) {
    FreeBlocks_.reserve(ReservedBlocsQuantity);
    pointer memory = Memory_;
    for( size_type i = 0; i < ReservedBlocsQuantity; ++i ) {
      FreeBlocks_.push_back(memory + i);
    }
  };

  ~CustomAllocator() {
    alloc_traits::deallocate(Allocator_, Memory_, ReservedBlocsQuantity);
  }

  pointer allocate(const size_type n) {
    if( n > FreeBlocks_.size() ) {
      throw std::runtime_error(std::to_string(n) + " > " + std::to_string(FreeBlocks_.size()));
    }
    for( size_type i = 0; i < n - 1; ++i ) {
      FreeBlocks_.pop_back();
    }
    pointer block = FreeBlocks_.back();
    FreeBlocks_.pop_back();

    return block;
  }

  void deallocate(value_type *p, const size_type n) noexcept {
    for( size_type i = 0; i < n; ++i ) {
      FreeBlocks_.push_back(p + i);
    }
  }
};