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
                                                     std::vector<size_t>&& params) {
  if( converterName == "mix" ) {
    return std::make_unique<conv::MixConverter>(std::move(params));
  }
  else if (converterName == "mute"){
    return std::make_unique<conv::MuteConverter>(std::move(params));
  }
  else if (converterName == "bass"){
    return std::make_unique<conv::BassBoostConverter>(std::move(params));
  }
  else if (converterName == "lowpass"){
    return std::make_unique<conv::LowPassConverter>(std::move(params));
  }
  else if (converterName == "copy"){
    return std::make_unique<conv::CopyConverter>(std::move(params));
  }
  else{
    throw conv::UnckonwConverter(converterName);
  }
}
