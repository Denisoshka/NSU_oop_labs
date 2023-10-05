#ifndef CL_PARSER_CL_PARSER_H
#define CL_PARSER_CL_PARSER_H

#include <iostream>
#include <vector>


class clParser {
private:
  std::string outputFileName_;
  std::string configFileName_;
  std::vector<std::string> inputFilesName_;


public:
  //  explicit clParser();
  bool getOptions(int argc, char **argv);
  void printConverterDesc();
};


#endif// CL_PARSER_CL_PARSER_H
