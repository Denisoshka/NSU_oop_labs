#include "converters.hpp"

/*
params{taskFinished(0), secondsStart(1), secondsEnd(2), changedSeconds(3)}
sample1 = FinalStream
sample2 = array of inputSamples
*/
void conv::MuteConverter::process(std::vector<int16_t>& sample1,
                                  const std::vector<std::vector<int16_t>>& samples,
                                  TaskInf& params) {
  //  size_t curSecond = params[1] + params[3];
  if( params.startTime <= params.curSec && params.curSec < params.endTime ) {
    memset(sample1.data(), 0, sample1.size() * sizeof(*sample1.data()));
  }
  ++params.curSec;
  //  params[0] = curSecond == params[2];
}
