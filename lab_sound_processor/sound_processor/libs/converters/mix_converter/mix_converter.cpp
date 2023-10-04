#include "converters.h"

void conv::MixConverter::process(sampleBuffer &sample1, std::vector<sampleBuffer&> &samples,
                           std::vector<size_t> &params) {
  for( int i = 0; i < sample1.curLen_; ++i ) {
    PrevSample2.sample_[i] = PrevSample2.sample_[i] / 2 + sample1.sample_[i] / 2;
  }
}
