#include "tuple_cxx11.hpp"

int main() {
  std::tuple<int, int, double, std::vector<std::string>> testTuple = {
          10, 10, 0.9, std::vector<std::string>{"saf", "sdafasgf", "dsafasdfas"}
  };
  std::cout << testTuple << std::endl;
  return 0;
}