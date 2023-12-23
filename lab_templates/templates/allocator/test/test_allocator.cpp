#include "allocator.hpp"

#include <sys/times.h>
#include <unistd.h>
#include <list>
#include <random>

const size_t gkReservedBlocksQuantity = 100000000;
const size_t gkCheckInd = 100;

template<bool DeallocateTest, class... Types>
void allocTestByTimes() {
  tms start{}, end{};

  long clocks_per_sec = sysconf(_SC_CLK_TCK);
  long clocks;

  std::list<Types...> testList{};
  times(&start);
  if constexpr( DeallocateTest ) {
    for( size_t i = 0; i != gkReservedBlocksQuantity; ++i ) {
      testList.push_back(random());
    }

    testList.clear();

    for( size_t i = 0; i != gkReservedBlocksQuantity; ++i ) {
      testList.push_back(random());
    }
  }
  else {
    for( size_t i = 0; i != gkReservedBlocksQuantity; ++i ) {
      testList.push_back(random());
    }
  }
  times(&end);
  clocks = end.tms_utime - start.tms_utime;

  std::cout << "time: " << static_cast<double>(clocks) / static_cast<double>(clocks_per_sec)
            << std::endl;
  if( *testList.begin() == gkCheckInd ) {
    std::cout << "lol\n";
  }
}

int main() {
  using value_type = long;
  allocTestByTimes<false, value_type,
                   CustomAllocator::PoolAllocator<value_type, gkReservedBlocksQuantity>>();
  allocTestByTimes<false, value_type>();


  return 0;
}