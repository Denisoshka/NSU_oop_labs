#include "csv_parser.hpp"

int main() {
  std::ifstream csvStream("/home/denis/CLionProjects/nsu_oop_labs/lab_templates/templates/"
                          "csv_parser/test/test.csv");
  // может быть любое количество любых типов
  parser::CSVParser<int, std::string, double> parser(csvStream, 0);
  //  std::tuple<int, std::string, double, std::vector<std::string>>

  for( const auto& rs: parser ) {
    std::cout << get<0>(rs) << " " << get<1>(rs) << " "<< get<2>(rs);
    for (const auto & other :  get<3>(rs)){
      std::cout <<" " <<other;
    }
    std::cout << std::endl;
//    rs;
    //    std::cout << rs << "\n";
  }
  return 0;
}