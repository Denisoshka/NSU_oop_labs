#include "flat_map.h"
#include "gtest/gtest.h"

int main(){
  FlatMap map;
  map["key1"]="value1";
  EXPECT_FALSE(map.contains("key1"));
  return 0;
}