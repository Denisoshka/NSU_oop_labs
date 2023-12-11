#include "csv_parser.hpp"

int main(int Argc, char ** Argv) {


  if (Argc == 1){
//    todo
  }
  std::ifstream csvStream{Argv[1]};
  parser::CSVParser<int, std::string, double, std::string, std::string, std::string, int> parser(csvStream, 0);
  for( const auto& rs: parser ) {
    using namespace tuple_cxx20;
    std::cout << rs << std::endl;
  }
  return 0;
}
