#include "converters.h"

void conv::MixConverter::process(sampleBuffer &AddSample, std::vector<sampleBuffer&> &samples,
                           std::vector<size_t> &params) {
  for( int i = 0; i < AddSample.curLen_; ++i ) {
    PrevSample2.sample_[i] = PrevSample2.sample_[i] / 2 + AddSample.sample_[i] / 2;
  }
}
