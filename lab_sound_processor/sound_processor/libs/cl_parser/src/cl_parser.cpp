#include "cl_parser.hpp"

#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;

bool CLParser::parseOptions(const int kArgc, char **argv,
                            boost::program_options::variables_map& vm) {
  po::options_description desc("General options");
  std::string task_type;
  desc.add_options()("help,h", "Show options description")(
          "config,C", po::value<std::string>(), "Configuration file")(
          "output,O", po::value<std::string>(), "Output file")(
          "input,I", po::value<std::vector<std::string>>()->multitoken(),
          "Input files");

  po::parsed_options parsed =
          po::command_line_parser(kArgc, argv).options(desc).allow_unregistered().run();
  po::store(parsed, vm);
  po::notify(vm);

  if( vm.count("help") || !vm.count("input") || !vm.count("output") ||  !vm.count("config") ) {
    return false;
  }
  else {
    return true;
  }
}
