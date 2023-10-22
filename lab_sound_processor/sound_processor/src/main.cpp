#include <boost/program_options.hpp>
#include <iostream>
#include <string>

#include "cl_parser.hpp"
#include "converters.hpp"
#include "process.hpp"
#include "process_exceptions.hpp"
#include "wav_exceptions.hpp"

int main(int argc, char **argv) {
  boost::program_options::variables_map VM;
  CLParser parser{};
  std::string kConvDesk = conv::GetConvertersDescription();
  if( !parser.parseOptions(argc, argv, VM) ) {
    process::printConverterDesc("sound processor", parser.getParseDescription(), kConvDesk);
    return 0;
  }
  process::Process soundProcessor{VM};

  try {
    soundProcessor.executeConversions();
  } catch( const WAV::WAVEexception& e ) {
    std::cerr << e.what();
    return e.getErrorCode();
  } catch( const conv::ConvertersException& e ) {
    std::cerr << e.what();
    return e.getErrorCode();
  } catch( const process::ProcessException& e ) {
    std::cerr << e.what();
    return e.getErrorCode();
  }

  return 0;
}
