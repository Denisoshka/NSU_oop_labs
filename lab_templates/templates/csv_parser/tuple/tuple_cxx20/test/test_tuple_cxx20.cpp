#include "tuple_cxx20.hpp"

int main() {
  using namespace tuple_cxx20;
  std::tuple<int, int, double, std::vector<std::string>> testTuple = {
          10, 10, 0.9, std::vector<std::string>{"saf", "sdafasgf", "dsafasdfas"}
  };
  std::cout << testTuple << std::endl;
  return 0;
}