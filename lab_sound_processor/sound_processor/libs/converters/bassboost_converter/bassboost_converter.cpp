#include <cstdint>
#include "bassboost_conveter.hpp"
#include "converters.hpp"

void conv::BassBoostConverter::process(std::vector<int16_t>& mainSample,
                                       const std::vector<int16_t>& kSubSample) {
  if( TaskInf_.CurTime < TaskInf_.EndTime ) {
    for( size_t i = 0; i < kSubSample.size(); ++i ) {
      int16_t sample = (kSubSample[i] < bassFactor_) ? kSubSample[i] : bassFactor_;
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
  ConverterWithAdditionalParams::setParams(kInStreams, kParams);
  if( OtherParams.size() == 2 ) {
    bassFactor_ = OtherParams[0];
    BassBoostCoeficent_ = OtherParams[1];
  }
  else if( OtherParams.size() == 1 ) {
    bassFactor_ = OtherParams[0];
  }
}

conv::BassBoostConverter::BassBoostConverter(const std::vector<size_t>& kInStreams,
                                             const std::vector<size_t>& kParams) {
  BassBoostConverter::setParams(kInStreams, kParams);
}
