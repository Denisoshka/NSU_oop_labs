#ifndef CL_PARSER_CL_PARSER_H
#define CL_PARSER_CL_PARSER_H

#include <iostream>
#include <vector>
#include "boost/program_options.hpp"

namespace po = boost::program_options;

class clParser {
  private:
  std::string outputFileName_;
  std::string configFileName_;
  std::vector<std::string> inputFileName_;


  public:
      po::options_description opts_desc("General options");
    opts_desc.add_options()
        ("help,h",
         "Show options description")
        ("config,c",
//  explicit clParser();
  void getOptions();


};


#endif// CL_PARSER_CL_PARSER_H
