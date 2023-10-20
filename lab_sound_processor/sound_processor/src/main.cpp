#include <boost/program_options.hpp>

#include "cl_parser.hpp"
#include "converters.hpp"
#include "process.hpp"

#include <string>

int main(int argc, char **argv) {
  boost::program_options::variables_map VM;
  CLParser parser{};
  std::string kConvDesk = conv::GetConvertersDescription();
  if( !parser.parseOptions(argc, argv, VM) ) {
    printConverterDesc("sound processor", parser.getParseDescription(), kConvDesk);
    return 0;
  }
  Process soundProcessor{VM};
  soundProcessor.executeConversions();

  return 0;
}
