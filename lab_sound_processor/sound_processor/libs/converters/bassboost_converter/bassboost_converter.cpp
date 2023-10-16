#include <cstdint>
#include "bassboost_conveter.hpp"
#include "converters.hpp"

void conv::BassBoostConverter::process(std::vector<int16_t>& mainSample,
                                       const std::vector<int16_t>& kSubSample) {
  if( taskInf_.curTime < taskInf_.endTime ) {
    for( size_t i = 0; i < kSubSample.size(); ++i ) {
      int16_t sample = (kSubSample[i] < bassFactor_) ? kSubSample[i] : bassFactor_;
      sample = (llabs(sample) * BassBoostCoeficent_ > INT16_MAX)
                     ? INT16_MAX * (llabs(sample) / bassFactor_)
                     : sample * BassBoostCoeficent_;
      mainSample[i] = sample;
    }
  }
  taskInf_.curTime++;
  taskInf_.taskFinished = taskInf_.curTime >= taskInf_.endTime;
}

void conv::BassBoostConverter::setParams(const std::vector<size_t>& kParams) {
  Converter::setParams(kParams);
  if( taskInf_.otherParams.size() == 2 ) {
    bassFactor_ = taskInf_.otherParams[0];
    BassBoostCoeficent_ = taskInf_.otherParams[1];
  }
  else if( taskInf_.otherParams.size() == 1 ) {
    bassFactor_ = taskInf_.otherParams[0];
  }
}

conv::BassBoostConverter::BassBoostConverter(const std::vector<size_t>& kParams) {
  BassBoostConverter::setParams(kParams);
}
