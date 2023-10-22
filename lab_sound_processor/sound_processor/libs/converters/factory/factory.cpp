#include "bassboost_conveter.hpp"
#include "converters.hpp"
#include "copy_converter.hpp"
#include "lowpass_converter.hpp"
#include "mix_converter.hpp"
#include "mute_converter.hpp"

#include <memory>

std::unique_ptr<conv::Converter> conv::makeConverter(const std::string& kConverterName,
                                                     const std::vector<size_t>& kParams) {
  if( kConverterName == "mix" ) {
    return std::make_unique<conv::MixConverter>(kParams);
  }
  else if( kConverterName == "mute" ) {
    return std::make_unique<conv::MuteConverter>(kParams);
  }
  else if( kConverterName == "bass" ) {
    return std::make_unique<conv::BassBoostConverter>(kParams);
  }
  else if( kConverterName == "lowpass" ) {
    return std::make_unique<conv::LowPassConverter>(kParams);
  }
  else if( kConverterName == "copy" ) {
    return std::make_unique<conv::CopyConverter>(kParams);
  }
  else {
    throw conv::UnknownConverter(kConverterName);
  }
}
