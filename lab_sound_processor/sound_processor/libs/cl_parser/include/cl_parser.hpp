#ifndef PROCESS_CL_PARSER_H
#define PROCESS_CL_PARSER_H

#include <boost/program_options.hpp>
#include <iostream>
#include <vector>

namespace po = boost::program_options;

class clParser {
public:
  clParser() = default;
  bool parseOptions(int argc, char** argv);
  void printConverterDesc();
  po::variables_map getVariablesMap();

private:
  po::variables_map VM_;
};


#endif// PROCESS_CL_PARSER_H
