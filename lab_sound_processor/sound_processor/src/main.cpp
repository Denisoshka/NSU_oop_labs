#include <boost/program_options.hpp>

#include "process.hpp"

namespace po = boost::program_options;

int main(int argc, char** argv) {
  po::variables_map VM;
  clParser parser{};
  if( !parser.parseOptions(argc, argv, VM) ) {
    parser.printConverterDesc();
    return 0;
  }
  process soundProcessor{VM};
  soundProcessor.executeConversions();

  return 0;
}
