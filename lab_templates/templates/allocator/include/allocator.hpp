#pragma once
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <vector>

namespace CustomAllocator {
  template<class T, size_t PoolSize = 1, size_t ChunkSize = 1>
  struct PoolAllocator {
    static_assert(!std::is_same_v<T, void>, "Type of the allocator can not be void");
    static_assert(ChunkSize == PoolSize || ChunkSize == 1, "Uncorrect parameters to pool");

  public:
    template<typename U>
    struct rebind {
      using other = PoolAllocator<U, PoolSize, ChunkSize>;
    };

    using value_type = T;
    using pointer = value_type *;
    using size_type = std::size_t;

  private:
    std::vector<pointer> FreeBlocks_{};
    std::unique_ptr<value_type, decltype([](pointer ptr) { ::operator delete(ptr); })> Pool_{};

  public:
    PoolAllocator()
        : Pool_(static_cast<pointer>(::operator new(sizeof(value_type) * PoolSize))) {
      FreeBlocks_.reserve(PoolSize);
      for( size_type i = 0; i < PoolSize; ++i ) {
        FreeBlocks_.push_back(Pool_.get() + i * ChunkSize);
      }
    }

    virtual ~PoolAllocator() = default;

    pointer allocate(const size_type n) {
      if constexpr( ChunkSize == PoolSize ) {
        if( n > FreeBlocks_.size() * ChunkSize ) {
          throw std::bad_alloc();
        }
      }
      else {
        if( n > FreeBlocks_.size() ) {
          throw std::bad_alloc();
        }
      }

      auto p = FreeBlocks_.back();
      FreeBlocks_.pop_back();
      return p;
    }

    void deallocate(pointer p, const size_type n) noexcept {
      for( size_type i = 0; i < n; ++i ) {
        FreeBlocks_.push_back(p + i * ChunkSize);
      }
    }
  };
}// namespace CustomAllocator
