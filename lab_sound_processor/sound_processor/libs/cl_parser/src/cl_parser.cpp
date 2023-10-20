#include "cl_parser.hpp"

#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;

bool CLParser::parseOptions(const int kArgc, char **Argv,
                            boost::program_options::variables_map& VM) {
  po::options_description desc("General options");
  std::string task_type;
  desc.add_options()("help,h", "Show options description")("config,C", po::value<std::string>(),
                                                           "Configuration file")(
          "output,O", po::value<std::string>(), "Output file")(
          "input,I", po::value<std::vector<std::string>>()->multitoken(), "Input files");

  po::parsed_options parsed =
          po::command_line_parser(kArgc, Argv).options(desc).allow_unregistered().run();
  po::store(parsed, VM);
  po::notify(VM);

  if( VM.count("help") || !VM.count("input") || !VM.count("output") || !VM.count("config") ) {
    return false;
  }
  else {
    return true;
  }
}

std::string CLParser::getParseDescription() {
  return "[-h] [-C config.txt -O output.wav -I input1.wav [-I input2.wav …]]";
}
