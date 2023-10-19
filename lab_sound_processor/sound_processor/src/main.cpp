#include <boost/program_options.hpp>

#include "cl_parser.hpp"
#include "converters.hpp"
#include "process.hpp"

#include <string>

const std::string kSoundProcessorUsage{"sound_processor [-h] "
                                      "[-C config.txt "
                                      "-O output.wav "
                                      "-I input1.wav [- I input2.wav …]]"};

int main(int argc, char **argv) {
  boost::program_options::variables_map VM;
  CLParser parser{};
  std::string kConvDesk = conv::GetConvertersDescription();
  if( !parser.parseOptions(argc, argv, VM) ) {
        printConverterDesc(kSoundProcessorUsage, kConvDesk);
    return 0;
  }
  Process soundProcessor{VM};
  soundProcessor.executeConversions();

  return 0;
}
