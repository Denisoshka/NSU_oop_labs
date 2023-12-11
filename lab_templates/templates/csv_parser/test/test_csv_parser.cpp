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

class csvStream{
public:
  csvStream(const std::string & x = ""){

  }
private:
  decltype(std::cin.rdbuf()) cinBuff = std::cin.rdbuf();
  std::ifstream in;
};

int main(int Argc, char **Argv) {
  /*if( Argc != 1 ) {
    csvStream = std::move(std::cin);
  }
  else{
    std::ifstream csvStream{
  }*/
  std::ifstream csvStream{"/home/denis/CLionProjects/nsu_oop_labs_try/lab_templates/templates/"
                          "csv_parser/test/test1.csv"};
//  std::ifstream csvStream;
//  auto buf = std::cin.rdbuf();
//  csvStream.rdbuf(std::cin.rdbuf());

  parser::CSVParser<Strategy, int, std::string, double> parser(csvStream, 0);
  for( const auto& rs: parser ) {
    using namespace tuple_cxx20;
    std::cout << rs << std::endl;
  }
  return 0;
}
