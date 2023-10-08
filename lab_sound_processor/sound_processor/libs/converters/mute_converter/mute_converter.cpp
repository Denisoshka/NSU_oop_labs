#include "converters.hpp"
/*
params{taskFinished(0), secondsStart(1), secondsEnd(2), changedSeconds(3)}
sample1 = FinalStream
sample2 = array of inputSamples
*/
void conv::MuteConverter::process(sampleBuffer& sample1, std::vector<sampleBuffer>& samples,
                                  std::vector<size_t>& params) {
//  size_t curSecond = params[1] + params[3];
  if(params[1] <= params[3] && params[3] < params[2] ) {
    memset(sample1.sample_, 0, sample1.curLen_);
  }
  ++params[3];
//  params[0] = curSecond == params[2];
}


