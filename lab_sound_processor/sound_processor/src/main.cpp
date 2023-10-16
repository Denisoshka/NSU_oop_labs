#include <boost/program_options.hpp>

#include "cl_parser.hpp"
#include "process.hpp"

int main(int argc, char** argv) {
  boost::program_options::variables_map VM;
  CLParser parser{};
  if( !parser.parseOptions(argc, argv, VM) ) {
    parser.printConverterDesc();
    return 0;
  }
  Process soundProcessor{VM};
  soundProcessor.executeConversions();

  return 0;
}
