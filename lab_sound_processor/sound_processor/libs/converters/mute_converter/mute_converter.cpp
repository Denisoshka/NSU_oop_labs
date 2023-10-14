#include "converters.hpp"
#include "mute_converter.hpp"

void conv::MuteConverter::process(std::vector<int16_t>& mainSample,
                                  std::vector<int16_t>& subSample) {
  if( taskInf_.startTime <= taskInf_.curTime && taskInf_.curTime < taskInf_.endTime ) {
    memset(mainSample.data(), 0, mainSample.size() * sizeof(*mainSample.data()));
  }
  ++taskInf_.curTime;
}

void conv::MuteConverter::setParams(conv::TaskParams&& params) {
  taskInf_ = std::move(params);
}

conv::MuteConverter::MuteConverter(conv::TaskParams&& params)
    : taskInf_(params) {
}
