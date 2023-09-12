#include "flat_map.h"
#include "gtest/gtest.h"

#include <iostream>

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(test_constructors, constructor_without_args) {
  FlatMap<std::string, std::string> tmp;
  EXPECT_EQ(tmp.size(), 0);
}


TEST(FlatMapTest, Erase_1) {
  std::vector<std::vector<std::string>> testCase
          = {{"key1", "value1"}, {"key2", "value2"}, {"key3", "value3"}, {"key4", "value4"},
             {"key5", "value5"}, {"key6", "value6"}, {"key7", "value7"}};
  FlatMap<std::string, std::string> testMap;
  for (const auto &params : testCase) {
    testMap[(std::string) params[0]] = params[1];
  }
  for (const auto &params : testCase) {
    EXPECT_EQ(testMap.erase(params[0]), 1);
    EXPECT_FALSE(testMap.contains(params[0])) << params[0];
  }
  EXPECT_EQ(testMap.size(), 0);
}

TEST(FlatMapTest, Erase_2) {
  std::vector<std::vector<std::string>> testCase
          = {{"key7", "value7"}, {"key6", "value6"}, {"key5", "value5"}, {"key4", "value4"},
             {"key3", "value3"}, {"key2", "value2"}, {"key1", "value1"}};
  FlatMap<std::string, std::string> testMap;
  for (const auto &params : testCase) {
    testMap[(std::string) params[0]] = params[1];
  }
  for (const auto &params : testCase) {
    EXPECT_EQ(testMap.erase(params[0]), 1);
    EXPECT_FALSE(testMap.contains(params[0]));
  }
  EXPECT_EQ(testMap.size(), 0);
}

TEST(FlatMapTest, Erase_3) {
  std::vector<std::vector<std::string>> testCase
          = {{"key7", "value7"}, {"key1", "value1"}, {"key6", "value6"}, {"key2", "value2"},
             {"key5", "value5"}, {"key3", "value3"}, {"key4", "value4"}};
  FlatMap<std::string, std::string> testMap;
  for (const auto &params : testCase) {
    testMap[(std::string) params[0]] = params[1];
  }
  for (const auto &params : testCase) {
    EXPECT_EQ(testMap.erase(params[0]), 1) << params[0];
    EXPECT_FALSE(testMap.contains(params[0])) << params[0];
  }
  EXPECT_EQ(testMap.size(), 0);
}

TEST(FlatMapTest, Insert_1) {
  std::vector<std::vector<std::string>> testCase
          = {{"key1", "value1"}, {"key2", "value2"}, {"key3", "value3"}, {"key4", "value4"},
             {"key5", "value5"}, {"key6", "value6"}, {"key7", "value7"}};
  FlatMap<std::string, std::string> testMap;
  for (const auto &params : testCase) {
    testMap[(std::string) params[0]] = params[1];
  }
  for (const auto &params : testCase) {
    EXPECT_EQ(testMap[params[0]], params[1]);
  }
}

TEST(FlatMapTest, Insert_2) {
  std::vector<std::vector<std::string>> testCase
          = {{"key7", "value7"}, {"key6", "value6"}, {"key5", "value5"}, {"key4", "value4"},
             {"key3", "value3"}, {"key2", "value2"}, {"key1", "value1"}};
  FlatMap<std::string, std::string> testMap;
  for (auto &params : testCase) {
    testMap[params[0]] = params[1];
  }
  for (auto &params : testCase) {
    EXPECT_EQ(testMap[params[0]], params[1]);
  }
}

TEST(FlatMapTest, Insert_3) {
  std::vector<std::vector<std::string>> testCase
          = {{"key1", "value1"}, {"key7", "value7"}, {"key2", "value2"}, {"key6", "value6"},
             {"key3", "value3"}, {"key5", "value5"}, {"key4", "value4"}};
  FlatMap<std::string, std::string> testMap;
  for (const auto &params : testCase) {
    testMap[(std::string) params[0]] = params[1];
  }
  for (const auto &params : testCase) {
    EXPECT_EQ(testMap[params[0]], params[1]);
  }
}

TEST(FlatMapTest, Insert_4) {
  std::vector<std::vector<std::string>> testCase
          = {{"key1", "value1"}, {"key2", "value2"}, {"key3", "value3"}, {"key5", "value5"},
             {"key6", "value6"}, {"key7", "value7"}, {"key4", "value4"}};
  FlatMap<std::string, std::string> testMap;
  for (const auto &params : testCase) {
    testMap[(std::string) params[0]] = params[1];
  }
  for (const auto &params : testCase) {
    EXPECT_EQ(testMap[params[0]], params[1]);
  }
}

TEST(FlatMapTest, Insert_5) {
  std::vector<std::vector<std::string>> testCase
          = {{"key7", "value7"}, {"key1", "value1"}, {"key6", "value6"}, {"key2", "value2"},
             {"key5", "value5"}, {"key3", "value3"}, {"key4", "value4"}};
  FlatMap<std::string, std::string> testMap;
  for (const auto &params : testCase) {
    testMap[(std::string) params[0]] = params[1];
  }
  for (const auto &params : testCase) {
    EXPECT_EQ(testMap[params[0]], params[1]);
  }
}

TEST(FlatMapTest, Clear) {
  std::vector<std::vector<std::string>> testCase
          = {{"key7", "value7"}, {"key1", "value1"}, {"key6", "value6"}, {"key2", "value2"},
             {"key5", "value5"}, {"key3", "value3"}, {"key4", "value4"}};
  FlatMap<std::string, std::string> testMap;
  for (const auto &params : testCase) {
    testMap[(std::string) params[0]] = params[1];
  }
  EXPECT_EQ(testMap.size(), testCase.size());
  testMap.clear();
  for (const auto &params : testCase) {
    EXPECT_FALSE(testMap.contains(params[0]));
  }
  EXPECT_EQ(testMap.size(), 0);
}


TEST(FlatMapTest, CopyConstructor) {
  FlatMap<std::string, std::string> testMap1;

  FlatMap testMap2 = testMap1;
  std::vector<std::vector<std::string>> testCase
          = {{"key7", "value7"}, {"key1", "value1"}, {"key6", "value6"}, {"key2", "value2"},
             {"key5", "value5"}, {"key3", "value3"}, {"key4", "value4"}};
  for (const auto &params : testCase) {
    testMap1[(std::string) params[0]] = params[1];
  }
  for (const auto &params : testCase) {
    EXPECT_NE(testMap1[params[0]], testMap2[params[0]]);
  }
  testMap2 = testMap1;
  for (const auto &params : testCase) {
    EXPECT_EQ(testMap1[params[0]], testMap2[params[0]]);
  }
}

TEST(FlatMapTest, Iterator) {
  FlatMap<std::string, std::string> tmp;

  std::vector<std::vector<std::string>> testCase
          = {{"key7", "value7"}, {"key1", "value1"}, {"key6", "value6"}, {"key2", "value2"},
             {"key5", "value5"}, {"key3", "value3"}, {"key4", "value4"}};
  for (const auto &params : testCase) {
    tmp[(std::string) params[0]] = params[1];
  }
  for (const auto &params : testCase) {
    EXPECT_EQ(tmp[params[0]], params[1]);
  }
  EXPECT_EQ(tmp.size(), 7);
}

TEST(FlatMapTest, Find_1) {
  FlatMap<std::string, std::string> testMap1;

  std::vector<std::vector<std::string>> testCase
          = {{"key7", "value7"}, {"key1", "value1"}, {"key6", "value6"}, {"key2", "value2"},
             {"key5", "value5"}, {"key3", "value3"}, {"key4", "value4"}};
  for (const auto &params : testCase) {
    testMap1[(std::string) params[0]] = params[1];
  }

  size_t count = 0;
  for (auto it = testMap1.find("key1"); it != testMap1.end(); ++it) {
    count++;
  }
  EXPECT_EQ(count, 7);

  count = 0;
  for (auto it = testMap1.find("key4"); it != testMap1.end(); ++it) {
    count++;
  }
  EXPECT_EQ(count, 4);

  count = 0;
  for (auto it = testMap1.find("key7"); it != testMap1.end(); ++it) {
    count++;
  }
  EXPECT_EQ(count, 1);

  count = 0;
  for (auto it = testMap1.find("key8"); it != testMap1.end(); ++it) {
    count++;
  }
  EXPECT_EQ(count, 0);
  EXPECT_FALSE(testMap1.contains("key8"));
}

TEST(FlatMapTest, Find_2) {
  FlatMap<int, int> tmp;

  std::vector<std::vector<int>> testCase = {{7, 7}, {1, 1}, {6, 6}, {2, 2}, {5, 5}, {3, 3}, {4, 4}};
  for (const auto &params : testCase) {
    tmp[params[0]] = params[1];
  }
  for (const auto &params : testCase) {
    EXPECT_EQ(tmp[params[0]], params[1]);
  }
  EXPECT_EQ(tmp.size(), 7);

  size_t count = 0;
  for (auto it = tmp.find(1); it != tmp.end(); ++it) {
    count++;
  }
  EXPECT_EQ(count, 7);

  count = 0;
  for (auto it = tmp.find(4); it != tmp.end(); ++it) {
    count++;
  }
  EXPECT_EQ(count, 4);

  count = 0;
  for (auto it = tmp.find(7); it != tmp.end(); ++it) {
    count++;
  }
  EXPECT_EQ(count, 1);

  count = 0;
  for (auto it = tmp.find(8); it != tmp.end(); ++it) {
    count++;
  }
  EXPECT_EQ(count, 0);
  EXPECT_FALSE(tmp.contains(8));
}
