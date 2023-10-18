#include "mute_converter.hpp"
#include "converters.hpp"

void conv::MuteConverter::process(std::vector<int16_t>& mainSample,
                                  const std::vector<int16_t>& kSubSample) {
  if( taskInf_.startTime <= taskInf_.curTime && taskInf_.curTime < taskInf_.endTime ) {
    std::fill(mainSample.begin(), mainSample.end(), 0);
  }
  ++taskInf_.curTime;
}

void conv::MuteConverter::setParams(const std::vector<size_t>& kParams) {
  Converter::setParams(kParams);
  taskInf_.startTime = 0;
}

conv::MuteConverter::MuteConverter(const std::vector<size_t>& kParams) {
  MuteConverter::setParams(kParams);
}
