#include "converters.hpp"

// #include
/*
params{taskFinished(0), secondsStart(1), secondsEnd(2), changedSeconds(3)}
sample1 = FinalStream
sample2 = array of inputSamples
*/
void conv::MixConverter::process(std::vector<int16_t>& sample1,
                                 const std::vector<std::vector<int16_t>>& samples,
                                 TaskInf& params) {
  params.curSec = (!params.curSec) ? params.startTime : params.curSec;

  if( params.startTime <= params.curSec && params.curSec < params.endTime ) {
    for( size_t i = 0; i < sample1.size(); ++i ) {
      sample1[i] = sample1[i] / 2 + samples[0][i] / 2;
    }
    params.curSec++;
  }

  params.taskFinished = params.curSec == params.endTime;
}
