#include "converters.hpp"

/*
params{taskFinished(0), secondsStart(1), secondsEnd(2), changedSeconds(3)}
sample1 = FinalStream
sample2 = array of inputSamples
*/
void conv::MuteConverter::process(std::vector<int16_t>& mainSample,
                                  std::vector<int16_t>& subSample) {
  if( taskInf_.startTime <= taskInf_.curSec && taskInf_.curSec < taskInf_.endTime ) {
    memset(mainSample.data(), 0, mainSample.size() * sizeof(*mainSample.data()));
  }
  ++taskInf_.curSec;
  //  params[0] = curSecond == params[2];
}
/*
void conv::MuteConverter::setParams(conv::TaskParams&& params) {
  taskInf_ = std::move(params);
}

size_t conv::MuteConverter::getReadSecond() {
  return taskInf_.curSec;
}

size_t conv::MuteConverter::getWriteSecond() {
  return taskInf_.curSec;
}
*/
