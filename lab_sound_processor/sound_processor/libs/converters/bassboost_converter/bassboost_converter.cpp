#include <cstdint>
#include "bassboost_conveter.hpp"
#include "converters.hpp"

void conv::BassBoostConverter::process(std::vector<int16_t>& mainSample,
                                       const std::vector<int16_t>& kSubSample) {
  if( TaskInf_.CurTime < TaskInf_.EndTime ) {
    auto& curSample = (InStream_) ? kSubSample: mainSample;
    for( size_t i = 0; i < kSubSample.size(); ++i ) {
      int16_t sample = (curSample[i] < bassFactor_) ? curSample[i] : bassFactor_;
      sample = (llabs(sample) * BassBoostCoeficent_ > INT16_MAX)
                     ? INT16_MAX * (llabs(sample) / bassFactor_)
                     : sample * BassBoostCoeficent_;
      mainSample[i] = sample;
    }
  }
  TaskInf_.CurTime++;
  TaskInf_.TaskFinished =
          TaskInf_.CurTime >= TaskInf_.EndTime || TaskInf_.CurTime >= TaskInf_.InDuration;
}

void conv::BassBoostConverter::setParams(const std::vector<size_t>& kInStreams,
                                         const std::vector<size_t>& kParams) {
  Converter::setParams(kInStreams, kParams);
  if( kParams.size() - kOtherParamsStart_ == 2 ) {
    bassFactor_ = kParams[kOtherParamsStart_];
    BassBoostCoeficent_ = kParams[kOtherParamsStart_ + 1];
  }
  else if( kParams.size() - kOtherParamsStart_ == 1 ) {
    bassFactor_ = kParams[kOtherParamsStart_];
  }
}

conv::BassBoostConverter::BassBoostConverter(const std::vector<size_t>& kInStreams,
                                             const std::vector<size_t>& kParams) {
  BassBoostConverter::setParams(kInStreams, kParams);
}
