#include "cl_parser.hpp"

#include <boost/program_options.hpp>

namespace po = boost::program_options;

bool clParser::parseOptions(int argc, char** argv) {
  po::options_description desc("General options");
  std::string task_type;
  desc.add_options()("help,h", "Show options description")(
          "config,C", po::value<std::string>(&SettingsFile_)->required(), "Configuration file")(
          "output,O", po::value<std::string>(&OutFile_)->required(), "Output file")(
          "input,I", po::value<std::vector<std::string>>(&InputFiles_)->multitoken()->required(),
          "Input files");

  po::variables_map vm;
  po::parsed_options parsed =
          po::command_line_parser(argc, argv).options(desc).allow_unregistered().run();
  po::store(parsed, vm);
  po::notify(vm);

  if( vm.count("help") ) {
    // todo add desc show
    return false;
  }
  else {
    VM_ = std::move(vm);
    return true;
  }
}

po::variables_map clParser::getVariablesMap() {
  return VM_;
}

void clParser::printConverterDesc() {
  std::cout << "sound_processor [-h] [-c config.txt output.wav input1.wav [input2.wav …]]"
            << std::endl;
}
