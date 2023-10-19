#ifndef PROCESS_CL_PARSER_H
#define PROCESS_CL_PARSER_H

#include <boost/program_options.hpp>

class CLParser {
public:
  CLParser() = default;
  bool parseOptions(const int kArgc, char **argv, boost::program_options::variables_map& vm);

private:
};


#endif// PROCESS_CL_PARSER_H
