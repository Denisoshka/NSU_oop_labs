#include <iostream>

#include "flat_map.h"
#include "gtest/gtest.h"


TEST(test_constructors, constructor_without_args) {
  FlatMap tmp;
  EXPECT_EQ(tmp.size(), 0);
}

TEST(FlatMapTest, InsertAndRetrieve) {
  FlatMap map;
  map["key1"] = "value1";
  map["key2"] = "value2";

  EXPECT_EQ(map["key1"], "value1");
  EXPECT_EQ(map["key2"], "value2");
}

TEST(FlatMapTest, Contains) {
  FlatMap map;
  map["key2"] = "value2";
  map["key3"] = "value3";

  EXPECT_FALSE(map.contains("key1"));
  EXPECT_TRUE(map.contains("key2"));
  EXPECT_TRUE(map.contains("key3"));
  map["key1"] = "value1";
  EXPECT_TRUE(map.contains("key1"));
  EXPECT_EQ(map.erase("key1"), 1);
  EXPECT_FALSE(map.contains("key1"));
}

TEST(FlatMapTest, CopyConstructor) {
  FlatMap map1;
  FlatMap map2 = map1;
  map1["key2"] = "value2";
  map1["key3"] = "value3";
  EXPECT_NE(map1["key2"], map2["key2"]);
  EXPECT_NE(map1["key3"], map2["key3"]);
  map2 = map1;
  EXPECT_EQ(map1["key2"], map2["key2"]);
  EXPECT_EQ(map1["key3"], map2["key3"]);
}


TEST(FlatMapTest, Fill) {
  FlatMap map;
  map["key1"] = "value1";
  map["key2"] = "value2";
  map["key3"] = "value3";
  EXPECT_EQ(map.size(), 3);
  map.clear();
  EXPECT_EQ(map.size(), 0);
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
