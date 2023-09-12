#include "flat_map.h"
#include "gtest/gtest.h"

#include <iostream>
#include "string"

int main(){
  FlatMap<int, int> map;
  map[1]=1;
  EXPECT_FALSE(map.contains(1));
  return 0;
}