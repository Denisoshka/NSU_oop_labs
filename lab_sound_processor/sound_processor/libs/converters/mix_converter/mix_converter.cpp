#include "converters.h"
/*
params{taskFinished(0), secondsStart(1), secondsEnd(2), changedSeconds(3)}
sample1 = FinalStream
sample2 = array of inputSamples
*/
void conv::MixConverter::process(sampleBuffer &sample1, std::vector<sampleBuffer> &samples,
                           std::vector<size_t> &params) {
  if (params[1] <= params[3] && params[3] < params[2]) {
    for( int i = 0; i < sample1.curLen_; ++i ) {
      sample1.sample_[i] = sample1.sample_[i] / 2 + samples[0].sample_[i] / 2;
    }
  }
  ++params[3];
  params[0] = params[3] == params[2] - params[1] + 1;
}
