#ifndef PROCESS_CL_PARSER_H
#define PROCESS_CL_PARSER_H

#include <iostream>
#include <vector>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

class clParser {
public:
  explicit clParser() = default;
  bool parseOptions(int argc, char **argv);
  void printConverterDesc();
  po::variables_map getVariablesMap();

private:
  std::string OutFile_;
  std::string SettingsFile_;
  std::vector<std::string> inputFiles_;
  po::variables_map vm_;
};


#endif// PROCESS_CL_PARSER_H
