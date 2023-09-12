#include "flat_map.h"
#include "gtest/gtest.h"

#include <iostream>
#include "string"

int main(){
  FlatMap<std::string, int> map;
  map["key1"]=1;
  EXPECT_FALSE(map.contains("key1"));
/*
  int array[] = {1, 2 ,3, 4, 5,6};
  for(auto i : array){
    std::cout<<i;
  }
*/


  size_t count = 0;
  for (auto it = map.find("key3"); it != map.end(); it++) {
    count++;
  }
  EXPECT_EQ(count, 0);


  return 0;



}