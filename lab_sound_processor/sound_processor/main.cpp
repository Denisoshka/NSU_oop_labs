#include "process/include/process.h"
#include "cl_parser/include/cl_parser.h"

#include <boost/program_options.hpp>

int main(int argc, char **argv){
  clParser parser{};
  if (!parser.parseOptions(argc, argv)){
    parser.printConverterDesc();
    return 0;
  }
  boost::program_options::variables_map vm = parser.getVariablesMap();
  process soundProcessor{vm};
  soundProcessor.executeConversions();

  return 0;
}
