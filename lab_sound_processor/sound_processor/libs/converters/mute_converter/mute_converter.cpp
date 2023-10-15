#include "mute_converter.hpp"
#include "converters.hpp"

void conv::MuteConverter::process(std::vector<int16_t>& mainSample,
                                  std::vector<int16_t>& subSample) {
  if( taskInf_.startTime <= taskInf_.curTime && taskInf_.curTime < taskInf_.endTime ) {
    std::fill(mainSample.begin(), mainSample.end(), 0);
  }
  ++taskInf_.curTime;
}

void conv::MuteConverter::setParams(std::vector<size_t>&& params) {
  taskInf_ = convertToCONVParams(std::move(params));
}

conv::MuteConverter::MuteConverter(std::vector<size_t>&& params) {
  MuteConverter::setParams(std::move(params));
}
