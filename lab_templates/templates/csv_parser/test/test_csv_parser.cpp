#include "csv_parser.hpp"

template<typename Type>
struct Strategy {
  auto operator()(const std::string& x) {
    if constexpr( std::is_floating_point<Type>::value ) {
      return std::stold(x);
    }
    else if constexpr( std::is_integral<Type>::value && std::is_signed<Type>::value ) {
      return std::stoll(x);
    }
    else if constexpr( std::is_integral<Type>::value && std::is_unsigned<Type>::value ) {
      return std::stoull(x);
    }
    else if constexpr( std::is_same<Type, std::string>::value ) {
      return x;
    }
  }
};

int main(int Argc, char **Argv) {
  parser::CSVParser<Strategy, int, std::string, double> parser(std::cin, 0);
  for( const auto& rs: parser ) {
    using namespace tuple_cxx20;
    std::cout << rs << std::endl;
  }
  return 0;
}
