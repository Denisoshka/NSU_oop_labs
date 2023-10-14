#include "factory.hpp"
#include "bassboost_conveter.hpp"
#include "converter.hpp"
#include "copy_converter.hpp"
#include "lowpass_converter.hpp"
#include "mix_converter.hpp"
#include "mute_converter.hpp"
#include "converter_exceptions.hpp"

#include <memory>

std::unique_ptr<conv::Converter> conv::makeConverter(const std::string& converterName,
                                                     TaskParams&& params) {
  if( converterName == "mix" ) {
    return std::make_unique<conv::MixConverter>(std::move(params));
  }
  else if (converterName == "mute"){
    return std::make_unique<conv::MuteConverter>();
  }
  else if (converterName == "bass"){
    return std::make_unique<conv::BassBoostConverter>();
  }
  else if (converterName == "lowpass"){
    return std::make_unique<conv::LowPassConverter>();
  }
  else if (converterName == "copy"){
    return std::make_unique<conv::CopyConverter>();
  }
  else{
    throw conv::UnckonwConverter(converterName);
  }
}
