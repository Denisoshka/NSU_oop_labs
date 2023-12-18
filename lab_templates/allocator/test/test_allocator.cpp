#include "allocator.hpp"

const size_t gkReservedBlocsQuantity = 100;
const size_t gkOtherReservedBlocsQuantity = 300;

struct A {
  A() {
    std::cout << "i born" << std::endl;
  }
};

int main() {
  //  std::vector<value_type, CustomAllocator<value_type, gkReservedBlocsQuantity>> test{};
  //  test.reserve(gkReservedBlocsQuantity);
  //  test.reserve(gkOtherReservedBlocsQuantity);

  {
    //    using value_type = std::pair<std::string, int>;
    using value_type = A;
    //std::vector<value_type, CustomAllocator<value_type, gkOtherReservedBlocsQuantity>> test{};
    //    const size_t gk = gkOtherReservedBlocsQuantity-1;
    //test.reserve(gkReservedBlocsQuantity);
    CustomAllocator<value_type, 10> alloc{};
//    for( size_t i = 0; i < gkReservedBlocsQuantity * 2; ++i ) {
//      test.push_back(A{});
//    }
/*
    for( const auto& x: test ) {
      std::cout << x.first << " " << x.second << std::endl;
    }*/
  }
  std::cout << "first test end\n\n" << std::endl;
  {/*
    std::cout << "second test___" << std::endl;
    using value_type = std::pair<std::string, int>;
    std::list<value_type, CustomAllocator<value_type, gkOtherReservedBlocsQuantity>> test{};
    for( size_t i = 0; i < gkOtherReservedBlocsQuantity; ++i ) {
      test.emplace_back(std::pair{"afsadf", gkReservedBlocsQuantity});
    }

    for( const auto& x: test ) {
      std::cout << x.first << " " << x.second << std::endl;
    }
    for( size_t i = 0; i < gkOtherReservedBlocsQuantity; ++i ) {
      test.pop_back();
    }
    for( size_t i = 0; i < gkOtherReservedBlocsQuantity; ++i ) {
      test.emplace_back(std::pair{"afsadf", gkReservedBlocsQuantity});
    }

    for( const auto& x: test ) {
      std::cout << x.first << " " << x.second << std::endl;
    }*/
  }
  {}

  return 0;
}