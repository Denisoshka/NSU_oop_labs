#include "converters.hpp"

/*
params{taskFinished(0), secondsStart(1), secondsEnd(2), changedSeconds(3)}
sample1 = FinalStream
sample2 = array of inputSamples
*/
void conv::MuteConverter::process(std::vector<int16_t>& mainSample,
                                  std::vector<int16_t>& subSample,
                                  TaskInf& params) {
  //  size_t curSecond = params[1] + params[3];
  if( params.startTime <= params.curSec && params.curSec < params.endTime ) {
    memset(mainSample.data(), 0, mainSample.size() * sizeof(*mainSample.data()));
  }
  ++params.curSec;
  //  params[0] = curSecond == params[2];
}
