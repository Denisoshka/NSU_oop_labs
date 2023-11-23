#include "allocator.hpp"

int main(){
  std::vector<std::string, CustomAllocator<std::string, 10>> test;



  return 0;
}