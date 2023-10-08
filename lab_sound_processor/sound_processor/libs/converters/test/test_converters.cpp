/*
#include "gtest/gtest.h"

#include "converters.hpp"

int main(int argc, char **argv){
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(test_1, test_parse) {
  std::vector<std::string > fileLinks {"input0.wav", "input1.wav" ,"input2.wav" };
  conv::ConverterInterface interface{};
  interface.setSettings("../test/config.txt", fileLinks);
  interface.curSec();
}
*/
