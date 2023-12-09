#include "flat_map.hpp"
#include "gtest/gtest.h"

#include <array>
#include <iostream>
#include <vector>


const std::vector<std::pair<std::string, std::string>> strTestCase1 = {
        {"key1", "value1"},
        {"key2", "value2"},
        {"key3", "value3"},
        {"key4", "value4"},
        {"key5", "value5"},
        {"key6", "value6"},
        {"key7", "value7"}
};
const std::vector<std::pair<std::string, std::string>> strTestCase2 = {
        {"key7", "value7"},
        {"key6", "value6"},
        {"key5", "value5"},
        {"key4", "value4"},
        {"key3", "value3"},
        {"key2", "value2"},
        {"key1", "value1"}
};
const std::vector<std::pair<std::string, std::string>> strTestCase3 = {
        {"key7", "value7"},
        {"key1", "value1"},
        {"key6", "value6"},
        {"key2", "value2"},
        {"key5", "value5"},
        {"key3", "value3"},
        {"key4", "value4"}
};

const std::vector<std::pair<std::string, std::string>> strTestCase4 = {
        {"key1", "value1"},
        {"key7", "value7"},
        {"key2", "value2"},
        {"key6", "value6"},
        {"key3", "value3"},
        {"key5", "value5"},
        {"key4", "value4"}
};

const std::vector<std::pair<std::string, std::string>> strTestCase5 = {
        {"key1", "value1"},
        {"key2", "value2"},
        {"key3", "value3"},
        {"key5", "value5"},
        {"key6", "value6"},
        {"key7", "value7"},
        {"key4", "value4"}
};


const std::vector<std::pair<int, int>> intTestCase1 = {
        {7, 7},
        {1, 1},
        {6, 6},
        {2, 2},
        {5, 5},
        {3, 3},
        {4, 4}
};

int main(int argc, char **argv) {
  std::map<int, int> a{};
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(test_constructors, constructor_without_args) {
  notStd::FlatMap<std::string, std::string> tmp;
  EXPECT_EQ(tmp.size(), 0);
}

TEST(FlatMapTest, Erase_1) {
  notStd::FlatMap<std::string, std::string> testMap;
  for( const auto& params: strTestCase1 ) {
    testMap[params.first] = params.second;
  }

  EXPECT_TRUE(testMap.contains(strTestCase1[0].first));
  for( const auto& params: strTestCase1 ) {
    EXPECT_EQ(testMap.erase(params.first), 1);
    EXPECT_FALSE(testMap.contains(params.first)) << params.first;
  }
  EXPECT_EQ(testMap.size(), 0);
}

TEST(FlatMapTest, Erase_2) {

  notStd::FlatMap<std::string, std::string> testMap;
  for( const auto& params: strTestCase2 ) {
    testMap[params.first] = params.second;
  }
  for( const auto& params: strTestCase2 ) {
    EXPECT_EQ(testMap.erase(params.first), 1);
    EXPECT_FALSE(testMap.contains(params.first));
  }
  EXPECT_EQ(testMap.size(), 0);
}

TEST(FlatMapTest, Erase_3) {

  notStd::FlatMap<std::string, std::string> testMap;
  for( const auto& params: strTestCase3 ) {
    testMap[params.first] = params.second;
  }
  for( const auto& params: strTestCase3 ) {
    EXPECT_EQ(testMap.erase(params.first), 1) << params.first;
    EXPECT_FALSE(testMap.contains(params.first)) << params.first;
  }
  EXPECT_EQ(testMap.size(), 0);
}

TEST(FlatMapTest, Erase_4) {

  notStd::FlatMap<std::string, std::string> testMap;
  for( const auto& params: strTestCase4 ) {
    testMap[params.first] = params.second;
  }
  for( const auto& params: strTestCase4 ) {
    EXPECT_EQ(testMap.erase(params.first), 1) << params.first;
    EXPECT_FALSE(testMap.contains(params.first)) << params.first;
  }
  EXPECT_EQ(testMap.size(), 0);
}

TEST(FlatMapTest, Erase_5) {

  notStd::FlatMap<std::string, std::string> testMap;
  for( const auto& params: strTestCase5 ) {
    testMap[params.first] = params.second;
  }
  for( const auto& params: strTestCase5 ) {
    EXPECT_EQ(testMap.erase(params.first), 1) << params.first;
    EXPECT_FALSE(testMap.contains(params.first)) << params.first;
  }
  EXPECT_EQ(testMap.size(), 0);
}

TEST(FlatMapTest, Insert_1) {

  notStd::FlatMap<std::string, std::string> testMap;
  for( const auto& params: strTestCase1 ) {
    testMap[params.first] = params.second;
  }
  for( const auto& params: strTestCase1 ) {
    EXPECT_EQ(testMap[params.first], params.second);
  }
}

TEST(FlatMapTest, Insert_2) {

  notStd::FlatMap<std::string, std::string> testMap;
  for( auto& params: strTestCase2 ) {
    testMap[params.first] = params.second;
  }
  for( auto& params: strTestCase2 ) {
    EXPECT_EQ(testMap[params.first], params.second);
  }
}

TEST(FlatMapTest, Insert_3) {
  notStd::FlatMap<std::string, std::string> testMap;
  for( const auto& params: strTestCase3 ) {
    testMap[params.first] = params.second;
  }
  for( const auto& params: strTestCase3 ) {
    EXPECT_EQ(testMap[params.first], params.second);
  }
}

TEST(FlatMapTest, Insert_4) {

  notStd::FlatMap<std::string, std::string> testMap;
  for( const auto& params: strTestCase4 ) {
    testMap[params.first] = params.second;
  }
  for( const auto& params: strTestCase4 ) {
    EXPECT_EQ(testMap[params.first], params.second);
  }
}

TEST(FlatMapTest, Insert_5) {

  notStd::FlatMap<std::string, std::string> testMap;
  for( const auto& params: strTestCase5 ) {
    testMap[params.first] = params.second;
  }
  for( const auto& params: strTestCase5 ) {
    EXPECT_EQ(testMap[params.first], params.second);
  }
}

TEST(FlatMapTest, Clear1) {

  notStd::FlatMap<std::string, std::string> testMap;
  for( const auto& params: strTestCase1 ) {
    testMap[params.first] = params.second;
  }
  EXPECT_EQ(testMap.size(), strTestCase1.size());
  testMap.clear();
  for( const auto& params: strTestCase1 ) {
    EXPECT_FALSE(testMap.contains(params.first));
  }
  EXPECT_EQ(testMap.size(), 0);
}

TEST(FlatMapTest, Clear2) {
  notStd::FlatMap<std::string, std::string> testMap;
  for( const auto& params: strTestCase2 ) {
    testMap[params.first] = params.second;
  }
  EXPECT_EQ(testMap.size(), strTestCase2.size());
  testMap.clear();
  for( const auto& params: strTestCase2 ) {
    EXPECT_FALSE(testMap.contains(params.first));
  }
  EXPECT_EQ(testMap.size(), 0);
}

TEST(FlatMapTest, Clear3) {

  notStd::FlatMap<std::string, std::string> testMap;
  for( const auto& params: strTestCase3 ) {
    testMap[params.first] = params.second;
  }
  EXPECT_EQ(testMap.size(), strTestCase3.size());
  testMap.clear();
  for( const auto& params: strTestCase3 ) {
    EXPECT_FALSE(testMap.contains(params.first));
  }
  EXPECT_EQ(testMap.size(), 0);
}

TEST(FlatMapTest, Clear4) {

  notStd::FlatMap<std::string, std::string> testMap;
  for( const auto& params: strTestCase4 ) {
    testMap[params.first] = params.second;
  }
  EXPECT_EQ(testMap.size(), strTestCase4.size());
  testMap.clear();
  for( const auto& params: strTestCase4 ) {
    EXPECT_FALSE(testMap.contains(params.first));
  }
  EXPECT_EQ(testMap.size(), 0);
}

TEST(FlatMapTest, CopyConstructor1) {
  notStd::FlatMap<std::string, std::string> testMap1;

  notStd::FlatMap testMap2 = testMap1;

  for( const auto& params: strTestCase1 ) {
    testMap1[params.first] = params.second;
  }
  for( const auto& params: strTestCase1 ) {
    auto key = testMap2[params.first];
    EXPECT_NE(testMap1[params.first], key);
  }
  testMap2 = testMap1;
  for( const auto& params: strTestCase1 ) {
    EXPECT_EQ(testMap1[params.first], testMap2[params.first]);
  }
}

TEST(FlatMapTest, CopyConstructor4) {
  notStd::FlatMap<std::string, std::string> testMap1;

  notStd::FlatMap testMap2 = testMap1;

  for( const auto& params: strTestCase4 ) {
    testMap1[params.first] = params.second;
  }
  for( const auto& params: strTestCase4 ) {
    EXPECT_NE(testMap1[params.first], testMap2[params.first]);
  }
  testMap2 = testMap1;
  for( const auto& params: strTestCase4 ) {
    EXPECT_EQ(testMap1[params.first], testMap2[params.first]);
  }
}

TEST(FlatMapTest, Iterator) {
  notStd::FlatMap<std::string, std::string> tmp;


  for( const auto& params: strTestCase1 ) {
    tmp[params.first] = params.second;
  }
  for( const auto& params: strTestCase1 ) {
    EXPECT_EQ(tmp[params.first], params.second);
  }
  EXPECT_EQ(tmp.size(), 7);
}

TEST(FlatMapTest, Find_1) {
  notStd::FlatMap<std::string, std::string> testMap1;
  for( const auto& params: strTestCase1 ) {
    testMap1[params.first] = params.second;
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
  notStd::FlatMap<int, int> tmp;


  for( const auto& params: intTestCase1 ) {
    tmp[params.first] = params.second;
  }
  for( const auto& params: intTestCase1 ) {
    EXPECT_EQ(tmp[params.first], params.second);
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
  notStd::FlatMap<int, std::string> tmp;

  std::array<int, 7> testCase_0 = {7, 1, 6, 2, 5, 3, 4};
  std::array<std::string, 7> testCase_1 = {"key7", "key1", "key6", "key2", "key5", "key3", "key4"};
  for( std::size_t i = 0; i < testCase_0.size(); i++ ) {
    tmp[testCase_0[i]] = testCase_1[i];
  }
  for( std::size_t i = 0; i < testCase_0.size(); i++ ) {
    EXPECT_EQ(tmp[testCase_0[i]], testCase_1[i]);
  }

  for( const auto& it: tmp ) {
    EXPECT_EQ(tmp[it.first], it.second);
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
  notStd::FlatMap<std::string, std::string> testMap1;

  for( const auto& params: strTestCase3 ) {
    testMap1[params.first] = params.second;
  }

  size_t count = 0;
  auto it = testMap1.begin();
  auto sub = it++;
  for( ; it != testMap1.end(); ++it ) {
    count++;
  }

  EXPECT_EQ(count, 6);
  count = 0;
  for( ; sub != testMap1.end(); ++sub ) {
    count++;
  }
  EXPECT_EQ(count, 7);
}

/*

TEST(FlatMapTest, try_emplace_test_1) {
  notStd::FlatMap<std::string, std::string> testMap1;
  std::vector<std::pair<std::string, std::string>> testCase = strTestCase3;
  for( auto&& x: testCase ) {
    auto it = (testMap1.try_emplace(x.first, std::move(x.second)));
    EXPECT_TRUE(it.second);
  }
  auto z = strTestCase3.begin();
  auto y = testCase.begin();
  for( ; z!= strTestCase3.end() && y != testCase.begin();++z, ++y ) {
    EXPECT_NE(z->first,y->first);
  }
  for( auto&& x: testMap1 ) {
    auto it = (testMap1.try_emplace(x.first, std::move(x.second)));
    EXPECT_FALSE(it.second);
  }
}
*/
