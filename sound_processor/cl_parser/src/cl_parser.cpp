#include "cl_parser.h"

#include <boost/program_options.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace po = boost::program_options;
namespace pt = boost::property_tree;


bool clParser::getOptions(int argc, char **argv) {
  po::options_description desc("General options");
  std::string task_type;
  desc.add_options()("help,h", "Show options description")(
          "config,c", po::value<std::string>(&configFileName_)->required(), "Configuration file")(
          "output,O", po::value<std::string>(&outputFileName_)->required(), "Output file")(
          "input,I",
          po::value<std::vector<std::string>>(&inputFilesName_)->multitoken()->required(),
          "Input files");

  po::variables_map vm;
  po::parsed_options parsed = po::command_line_parser(argc, argv).options(desc).allow_unregistered().run();
  po::store(parsed, vm);
  po::notify(vm);

  if (vm.count("help")){

    std::cout << desc << std::endl;
    printConverterDesc();

    return false;
  }
}

void clParser::printConverterDesc(){

}
