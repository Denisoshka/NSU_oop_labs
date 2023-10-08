#include <boost/program_options.hpp>

#include "process.hpp"

namespace po = boost::program_options;

int main(int argc, char** argv) {
  clParser parser{};
  if( !parser.parseOptions(argc, argv) ) {
    parser.printConverterDesc();
    return 0;
  }
  po::variables_map vm = parser.getVariablesMap();
  process soundProcessor{vm};
  soundProcessor.executeConversions();

  return 0;
}
