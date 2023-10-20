#ifndef PROCESS_CL_PARSER_H
#define PROCESS_CL_PARSER_H

#include <boost/program_options.hpp>

class CLParser {
public:
  CLParser() = default;
  bool parseOptions(const int kArgc, char **Argv, boost::program_options::variables_map& VM);
  std::string getParseDescription();
private:
};


#endif// PROCESS_CL_PARSER_H
