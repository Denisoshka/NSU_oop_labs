#include "cl_parser.h"

#include <boost/program_options.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace po = boost::program_options;
namespace pt = boost::property_tree;

bool clParser::parseOptions(int argc, char **argv) {
  po::options_description desc("General options");
  std::string task_type;
  desc.add_options()("help,h", "Show options description")(
          "config,c", po::value<std::string>(&SettingsFile_)->required(), "Configuration file")(
          "output,O", po::value<std::string>(&OutFile_)->required(), "Output file")(
          "input,I",
          po::value<std::vector<std::string>>(&inputFiles_)->multitoken()->required(),
          "Input files");

  po::variables_map vm;
  po::parsed_options parsed = po::command_line_parser(argc, argv).options(desc).allow_unregistered().run();
  po::store(parsed, vm);
  po::notify(vm);

  if( vm.count("help") ) {
    // todo add desc show
    std::cout << "sound_processor [-h] [-c config.txt output.wav input1.wav [input2.wav …]]" << std::endl;
    return false;
  }
  else{
    return true;
  }

}

void clParser::printConverterDesc() {
}
