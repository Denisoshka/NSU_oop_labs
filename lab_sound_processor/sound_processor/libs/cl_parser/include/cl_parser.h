#ifndef PROCESS_CL_PARSER_H
#define PROCESS_CL_PARSER_H

#include <iostream>
#include <vector>

class clParser {
public:
    explicit clParser();
  bool parseOptions(int argc, char **argv);
  //  void printConverterDesc();

private:
  std::string OutFile_;
  std::string SettingsFile_;
  std::vector<std::string> inputFiles_;
};


#endif// PROCESS_CL_PARSER_H
