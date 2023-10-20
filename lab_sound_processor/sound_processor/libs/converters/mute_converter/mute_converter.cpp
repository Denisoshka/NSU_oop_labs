#include "mute_converter.hpp"
#include "converters.hpp"

void conv::MuteConverter::process(std::vector<int16_t>& MainSample,
                                  const std::vector<int16_t>& kSubSample) {
  if( TaskInf_.StartTime <= TaskInf_.CurTime && TaskInf_.CurTime < TaskInf_.EndTime ) {
    std::fill(MainSample.begin(), MainSample.end(), 0);
  }
  ++TaskInf_.CurTime;
}

void conv::MuteConverter::setParams(const std::vector<size_t>& kParams) {
  Converter::setParams(kParams);
  TaskInf_.StartTime = 0;
}

conv::MuteConverter::MuteConverter(const std::vector<size_t>& kParams) {
  MuteConverter::setParams(kParams);
}
