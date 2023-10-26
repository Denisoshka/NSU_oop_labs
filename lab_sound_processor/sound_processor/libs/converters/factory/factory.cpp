#include "bassboost_conveter.hpp"
#include "converters.hpp"
#include "copy_converter.hpp"
#include "lowpass_converter.hpp"
#include "mix_converter.hpp"
#include "mute_converter.hpp"

#include <memory>

std::unique_ptr<conv::Converter> conv::makeConverter(const std::string& kConverterName,
                                                     const std::vector<size_t>& kInStreams,
                                                     const std::vector<size_t>& kParams) {
  if( kConverterName == "mix" ) {
    return std::make_unique<conv::MixConverter>(kInStreams, kParams);
  }
  else if( kConverterName == "mute" ) {
    return std::make_unique<conv::MuteConverter>(kInStreams, kParams);
  }
  else if( kConverterName == "bass" ) {
    return std::make_unique<conv::BassBoostConverter>(kInStreams, kParams);
  }
  else if( kConverterName == "lowpass" ) {
    return std::make_unique<conv::LowPassConverter>(kInStreams, kParams);
  }
  else if( kConverterName == "copy" ) {
    return std::make_unique<conv::CopyConverter>(kInStreams, kParams);
  }
  else {
    throw conv::UnknownConverter(kConverterName);
  }
}
