#include "factory.hpp"
#include "bassboost_conveter.hpp"
#include "converter.hpp"
#include "converter_exceptions.hpp"
#include "copy_converter.hpp"
#include "lowpass_converter.hpp"
#include "mix_converter.hpp"
#include "mute_converter.hpp"

#include <memory>

std::unique_ptr<conv::Converter> conv::makeConverter(const std::string& kCconverterName,
                                                     const std::vector<size_t>& kParams) {
  if( kCconverterName == "mix" ) {
    return std::make_unique<conv::MixConverter>(kParams);
  }
  else if( kCconverterName == "mute" ) {
    return std::make_unique<conv::MuteConverter>(kParams);
  }
  else if( kCconverterName == "bass" ) {
    return std::make_unique<conv::BassBoostConverter>(kParams);
  }
  else if( kCconverterName == "lowpass" ) {
    return std::make_unique<conv::LowPassConverter>(kParams);
  }
  else if( kCconverterName == "copy" ) {
    return std::make_unique<conv::CopyConverter>(kParams);
  }
  else {
    throw conv::UnckownConverter(kCconverterName);
  }
}
