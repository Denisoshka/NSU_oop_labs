#include "cl_parser.h"

void clParser::getOptions() {
  po::options_description opts_desc("General options");
    opts_desc.add_options()
        ("help,h",
         "Show options description")
        ("config,c",
}
