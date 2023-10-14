#ifndef PROCESS_CL_PARSER_H
#define PROCESS_CL_PARSER_H

#include <boost/program_options.hpp>

class clParser {
public:
  clParser() = default;
  bool parseOptions(int argc, char** argv, boost::program_options::variables_map& vm);
  void printConverterDesc();
  boost::program_options::variables_map getVariablesMap();

private:
};


#endif// PROCESS_CL_PARSER_H
