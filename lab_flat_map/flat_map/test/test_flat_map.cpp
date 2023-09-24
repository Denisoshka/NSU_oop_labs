#include "flat_map.h"
#include "gtest/gtest.h"

#include <array>
#include <iostream>
#include <vector>

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

std::vector<std::vector<std::string>> strTestCase1 = {
        {"key1", "value1"},
        {"key2", "value2"},
        {"key3", "value3"},
        {"key4", "value4"},
        {"key5", "value5"},
        {"key6", "value6"},
        {"key7", "value7"}
};
std::vector<std::vector<std::string>> strTestCase2 = {
        {"key7", "value7"},
        {"key6", "value6"},
        {"key5", "value5"},
        {"key4", "value4"},
        {"key3", "value3"},
        {"key2", "value2"},
        {"key1", "value1"}
};
std::vector<std::vector<std::string>> strTestCase3 = {
        {"key7", "value7"},
        {"key1", "value1"},
        {"key6", "value6"},
        {"key2", "value2"},
        {"key5", "value5"},
        {"key3", "value3"},
        {"key4", "value4"}
};

std::vector<std::vector<std::string>> strTestCase4 = {
        {"key1", "value1"},
        {"key7", "value7"},
        {"key2", "value2"},
        {"key6", "value6"},
        {"key3", "value3"},
        {"key5", "value5"},
        {"key4", "value4"}
};

std::vector<std::vector<std::string>> strTestCase5 = {
        {"key1", "value1"},
        {"key2", "value2"},
        {"key3", "value3"},
        {"key5", "value5"},
        {"key6", "value6"},
        {"key7", "value7"},
        {"key4", "value4"}
};

std::vector<std::vector<int>> intTestCase1 = {
        {7, 7},
        {1, 1},
        {6, 6},
        {2, 2},
        {5, 5},
        {3, 3},
        {4, 4}
};

TEST(test_constructors, constructor_without_args) {
  FlatMap<std::string, std::string> tmp;
  EXPECT_EQ(tmp.size(), 0);
}

TEST(FlatMapTest, Erase_1) {
  FlatMap<std::string, std::string> testMap;
  for( const auto &params: strTestCase1 ) {
    testMap[params[0]] = params[1];
  }

  EXPECT_TRUE(testMap.contains(strTestCase1[0][0]));
  for( const auto &params: strTestCase1 ) {
    EXPECT_EQ(testMap.erase(params[0]), 1);
    EXPECT_FALSE(testMap.contains(params[0])) << params[0];
  }
  EXPECT_EQ(testMap.size(), 0);
}

TEST(FlatMapTest, Erase_2) {

  FlatMap<std::string, std::string> testMap;
  for( const auto &params: strTestCase2 ) {
    testMap[(std::string)params[0]] = params[1];
  }
  for( const auto &params: strTestCase2 ) {
    EXPECT_EQ(testMap.erase(params[0]), 1);
    EXPECT_FALSE(testMap.contains(params[0]));
  }
  EXPECT_EQ(testMap.size(), 0);
}

TEST(FlatMapTest, Erase_3) {

  FlatMap<std::string, std::string> testMap;
  for( const auto &params: strTestCase3 ) {
    testMap[(std::string)params[0]] = params[1];
  }
  for( const auto &params: strTestCase3 ) {
    EXPECT_EQ(testMap.erase(params[0]), 1) << params[0];
    EXPECT_FALSE(testMap.contains(params[0])) << params[0];
  }
  EXPECT_EQ(testMap.size(), 0);
}

TEST(FlatMapTest, Erase_4) {

  FlatMap<std::string, std::string> testMap;
  for( const auto &params: strTestCase4 ) {
    testMap[(std::string)params[0]] = params[1];
  }
  for( const auto &params: strTestCase4 ) {
    EXPECT_EQ(testMap.erase(params[0]), 1) << params[0];
    EXPECT_FALSE(testMap.contains(params[0])) << params[0];
  }
  EXPECT_EQ(testMap.size(), 0);
}

TEST(FlatMapTest, Erase_5) {

  FlatMap<std::string, std::string> testMap;
  for( const auto &params: strTestCase5 ) {
    testMap[(std::string)params[0]] = params[1];
  }
  for( const auto &params: strTestCase5 ) {
    EXPECT_EQ(testMap.erase(params[0]), 1) << params[0];
    EXPECT_FALSE(testMap.contains(params[0])) << params[0];
  }
  EXPECT_EQ(testMap.size(), 0);
}

TEST(FlatMapTest, Insert_1) {

  FlatMap<std::string, std::string> testMap;
  for( const auto &params: strTestCase1 ) {
    testMap[(std::string)params[0]] = params[1];
  }
  for( const auto &params: strTestCase1 ) {
    EXPECT_EQ(testMap[params[0]], params[1]);
  }
}

TEST(FlatMapTest, Insert_2) {

  FlatMap<std::string, std::string> testMap;
  for( auto &params: strTestCase2 ) {
    testMap[params[0]] = params[1];
  }
  for( auto &params: strTestCase2 ) {
    EXPECT_EQ(testMap[params[0]], params[1]);
  }
}

TEST(FlatMapTest, Insert_3) {
  FlatMap<std::string, std::string> testMap;
  for( const auto &params: strTestCase3 ) {
    testMap[(std::string)params[0]] = params[1];
  }
  for( const auto &params: strTestCase3 ) {
    EXPECT_EQ(testMap[params[0]], params[1]);
  }
}

TEST(FlatMapTest, Insert_4) {

  FlatMap<std::string, std::string> testMap;
  for( const auto &params: strTestCase4 ) {
    testMap[(std::string)params[0]] = params[1];
  }
  for( const auto &params: strTestCase4 ) {
    EXPECT_EQ(testMap[params[0]], params[1]);
  }
}

TEST(FlatMapTest, Insert_5) {

  FlatMap<std::string, std::string> testMap;
  for( const auto &params: strTestCase5 ) {
    testMap[(std::string)params[0]] = params[1];
  }
  for( const auto &params: strTestCase5 ) {
    EXPECT_EQ(testMap[params[0]], params[1]);
  }
}

TEST(FlatMapTest, Clear1) {

  FlatMap<std::string, std::string> testMap;
  for( const auto &params: strTestCase1 ) {
    testMap[(std::string)params[0]] = params[1];
  }
  EXPECT_EQ(testMap.size(), strTestCase1.size());
  testMap.clear();
  for( const auto &params: strTestCase1 ) {
    EXPECT_FALSE(testMap.contains(params[0]));
  }
  EXPECT_EQ(testMap.size(), 0);
}

TEST(FlatMapTest, Clear2) {
  FlatMap<std::string, std::string> testMap;
  for( const auto &params: strTestCase2 ) {
    testMap[(std::string)params[0]] = params[1];
  }
  EXPECT_EQ(testMap.size(), strTestCase2.size());
  testMap.clear();
  for( const auto &params: strTestCase2 ) {
    EXPECT_FALSE(testMap.contains(params[0]));
  }
  EXPECT_EQ(testMap.size(), 0);
}

TEST(FlatMapTest, Clear3) {

  FlatMap<std::string, std::string> testMap;
  for( const auto &params: strTestCase3 ) {
    testMap[(std::string)params[0]] = params[1];
  }
  EXPECT_EQ(testMap.size(), strTestCase3.size());
  testMap.clear();
  for( const auto &params: strTestCase3 ) {
    EXPECT_FALSE(testMap.contains(params[0]));
  }
  EXPECT_EQ(testMap.size(), 0);
}

TEST(FlatMapTest, Clear4) {

  FlatMap<std::string, std::string> testMap;
  for( const auto &params: strTestCase4 ) {
    testMap[(std::string)params[0]] = params[1];
  }
  EXPECT_EQ(testMap.size(), strTestCase4.size());
  testMap.clear();
  for( const auto &params: strTestCase4 ) {
    EXPECT_FALSE(testMap.contains(params[0]));
  }
  EXPECT_EQ(testMap.size(), 0);
}

TEST(FlatMapTest, CopyConstructor1) {
  FlatMap<std::string, std::string> testMap1;

  FlatMap testMap2 = testMap1;

  for( const auto &params: strTestCase1 ) {
    testMap1[(std::string)params[0]] = params[1];
  }
  for( const auto &params: strTestCase1 ) {
    EXPECT_NE(testMap1[params[0]], testMap2[params[0]]);
  }
  testMap2 = testMap1;
  for( const auto &params: strTestCase1 ) {
    EXPECT_EQ(testMap1[params[0]], testMap2[params[0]]);
  }
}

TEST(FlatMapTest, CopyConstructor4) {
  FlatMap<std::string, std::string> testMap1;

  FlatMap testMap2 = testMap1;

  for( const auto &params: strTestCase4 ) {
    testMap1[(std::string)params[0]] = params[1];
  }
  for( const auto &params: strTestCase4 ) {
    EXPECT_NE(testMap1[params[0]], testMap2[params[0]]);
  }
  testMap2 = testMap1;
  for( const auto &params: strTestCase4 ) {
    EXPECT_EQ(testMap1[params[0]], testMap2[params[0]]);
  }
}

TEST(FlatMapTest, Iterator) {
  FlatMap<std::string, std::string> tmp;


  for( const auto &params: strTestCase1 ) {
    tmp[(std::string)params[0]] = params[1];
  }
  for( const auto &params: strTestCase1 ) {
    EXPECT_EQ(tmp[params[0]], params[1]);
  }
  EXPECT_EQ(tmp.size(), 7);
}

TEST(FlatMapTest, Find_1) {
  FlatMap<std::string, std::string> testMap1;
  for( const auto &params: strTestCase1 ) {
    testMap1[(std::string)params[0]] = params[1];
  }

  size_t count = 0;
  for( auto it = testMap1.find("key1"); it != testMap1.end(); ++it ) {
    count++;
  }
  EXPECT_EQ(count, 7);

  count = 0;
  for( auto it = testMap1.find("key4"); it != testMap1.end(); ++it ) {
    count++;
  }
  EXPECT_EQ(count, 4);

  count = 0;
  for( auto it = testMap1.find("key7"); it != testMap1.end(); ++it ) {
    count++;
  }
  EXPECT_EQ(count, 1);

  count = 0;
  for( auto it = testMap1.find("key8"); it != testMap1.end(); ++it ) {
    count++;
  }
  EXPECT_EQ(count, 0);
  EXPECT_FALSE(testMap1.contains("key8"));
}

TEST(FlatMapTest, Find_2) {
  FlatMap<int, int> tmp;


  for( const auto &params: intTestCase1 ) {
    tmp[params[0]] = params[1];
  }
  for( const auto &params: intTestCase1 ) {
    EXPECT_EQ(tmp[params[0]], params[1]);
  }
  EXPECT_EQ(tmp.size(), 7);

  size_t count = 0;
  for( auto it = tmp.find(1); it != tmp.end(); ++it ) {
    count++;
  }
  EXPECT_EQ(count, 7);

  count = 0;
  for( auto it = tmp.find(4); it != tmp.end(); ++it ) {
    count++;
  }
  EXPECT_EQ(count, 4);

  count = 0;
  for( auto it = tmp.find(7); it != tmp.end(); ++it ) {
    count++;
  }
  EXPECT_EQ(count, 1);

  count = 0;
  for( auto it = tmp.find(8); it != tmp.end(); ++it ) {
    count++;
  }
  EXPECT_EQ(count, 0);
  EXPECT_FALSE(tmp.contains(8));
}

TEST(FlatMapTest, Find_3) {
  FlatMap<int, std::string> tmp;

  std::array<int, 7> testCase_0 = {7, 1, 6, 2, 5, 3, 4};
  std::array<std::string, 7> testCase_1 = {"key7", "key1", "key6", "key2", "key5", "key3", "key4"};
  for( std::size_t i = 0; i < testCase_0.size(); i++ ) {
    tmp[testCase_0[i]] = testCase_1[i];
  }
  for( std::size_t i = 0; i < testCase_0.size(); i++ ) {
    EXPECT_EQ(tmp[testCase_0[i]], testCase_1[i]);
  }

  for( auto it: tmp ) {
    EXPECT_EQ(tmp[it.key], it.value);
  }

  EXPECT_EQ(tmp.size(), 7);

  size_t count = 0;
  for( auto it = tmp.find(1); it != tmp.end(); ++it ) {
    count++;
  }
  EXPECT_EQ(count, 7);

  count = 0;
  for( auto it = tmp.find(4); it != tmp.end(); ++it ) {
    count++;
  }
  EXPECT_EQ(count, 4);

  count = 0;
  for( auto it = tmp.find(7); it != tmp.end(); ++it ) {
    count++;
  }
  EXPECT_EQ(count, 1);

  count = 0;
  for( auto it = tmp.find(8); it != tmp.end(); ++it ) {
    count++;
  }
  EXPECT_EQ(count, 0);
  EXPECT_FALSE(tmp.contains(8));
}


TEST(FlatMapTest, postfix_incr) {
  FlatMap<std::string, std::string> testMap1;

  std::vector<std::vector<std::string>> testCase
          = {{"key7", "value7"}, {"key1", "value1"}, {"key6", "value6"}, {"key2", "value2"},
             {"key5", "value5"}, {"key3", "value3"}, {"key4", "value4"}};
  for (const auto &params : testCase) {
    testMap1[ params[0]] = params[1];
  }

  size_t count = 0;
  auto it = testMap1.begin();
  auto sub = it++;
  for (; it != testMap1.end(); ++it) {
    count++;
  }

  EXPECT_EQ(count, 6);
  count = 0;
  for (; sub != testMap1.end(); ++sub) {
    count++;
  }
  EXPECT_EQ(count, 7);
}

