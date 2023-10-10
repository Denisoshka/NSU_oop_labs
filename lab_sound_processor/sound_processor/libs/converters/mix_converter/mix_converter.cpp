#include "converters.hpp"

void conv::MixConverter::process(std::vector<int16_t>& mainSample,
                                 std::vector<int16_t>& subSample) {
  if( taskInf_.startTime <= taskInf_.curSec && taskInf_.curSec < taskInf_.endTime ) {
    for( size_t i = 0; i < mainSample.size(); ++i ) {
      mainSample[i] = mainSample[i] / 2 + subSample[i] / 2;
    }
  }
  taskInf_.curSec++;
  taskInf_.taskFinished = taskInf_.curSec >= taskInf_.endTime;
}

void conv::MixConverter::setParams(conv::TaskParams&& params) {
  taskInf_ = std::move(params);
  taskInf_.curSec = (!taskInf_.curSec) ? taskInf_.startTime : taskInf_.curSec;
}
/*
size_t conv::MixConverter::getReadSecond() {
  return taskInf_.curSec;
}

size_t conv::MixConverter::getWriteSecond() {
  return taskInf_.curSec;
}*/
