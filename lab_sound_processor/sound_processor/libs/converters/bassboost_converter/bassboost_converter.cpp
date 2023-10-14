#include <cstdint>
#include "converters.hpp"

void conv::BassBoostConverter::process(std::vector<int16_t>& mainSample,
                                       std::vector<int16_t>& subSample) {
  if( taskInf_.curTime < taskInf_.endTime ) {
    for( size_t i = 0; i < subSample.size(); ++i ) {
      int16_t sample = (subSample[i] < bassFactor_) ? subSample[i] : bassFactor_;
      sample = (llabs(sample) * BassBoostCoeficent_ > INT16_MAX)
              ? INT16_MAX * (llabs(sample) / bassFactor_) : sample * BassBoostCoeficent_;
      mainSample[i] = sample;
    }
  }
  taskInf_.curTime++;
  taskInf_.taskFinished = taskInf_.curTime >= taskInf_.endTime;
}

size_t conv::BassBoostConverter::getReadSecond() {
  return taskInf_.curTime;
}

size_t conv::BassBoostConverter::getWriteSecond() {
  return taskInf_.curTime;
}

void conv::BassBoostConverter::setParams(conv::TaskParams&& params) {
  taskInf_ = params;
  taskInf_.curTime = taskInf_.startTime;
  if (taskInf_.otherParams.size() == 2){
    bassFactor_ = taskInf_.otherParams[0];
    BassBoostCoeficent_ = taskInf_.otherParams[1];
  }
  else if (taskInf_.otherParams.size() == 1){
    bassFactor_ = taskInf_.otherParams[0];
  }
}

bool conv::BassBoostConverter::taskFinished() {
  return taskInf_.taskFinished;
}

size_t conv::BassBoostConverter::getReadStream() {
  return taskInf_.stream;
}
