#include "cl_parser.hpp"

#include <boost/program_options.hpp>

namespace po = boost::program_options;

bool clParser::parseOptions(int argc, char** argv, po::variables_map& VM) {
  po::options_description desc("General options");
  std::string task_type;
  desc.add_options()("help,h", "Show options description")(
          "config,C", po::value<std::string>()->required(), "Configuration file")(
          "output,O", po::value<std::string>()->required(), "Output file")(
          "input,I", po::value<std::vector<std::string>>()->multitoken()->required(),
          "Input files");

  po::parsed_options parsed =
          po::command_line_parser(argc, argv).options(desc).allow_unregistered().run();
  po::store(parsed, VM );
  po::notify(VM);

  if( VM.count("help") ) {
    // todo add desc show
    return false;
  }
  else {
    return true;
  }
}

void clParser::printConverterDesc() {
  std::cout << "sound_processor [-h] [-c config.txt output.wav input1.wav [input2.wav …]]"
            << std::endl;
}
