#include "converters.hpp"

void conv::MixConverter::process(std::vector<int16_t>& mainSample,
                                 std::vector<int16_t>& subSample) {
  if( taskInf_.curSec < taskInf_.endTime ) {
    for( size_t i = 0; i < mainSample.size(); ++i ) {
      mainSample[i] = mainSample[i] / 2 + subSample[i] / 2;
    }
  }
  taskInf_.curSec++;
  taskInf_.taskFinished = taskInf_.curSec >= taskInf_.endTime;
}

void conv::MixConverter::setParams(conv::TaskParams& params) {
  taskInf_ = params;
  taskInf_.curSec = taskInf_.startTime;
}

size_t conv::MixConverter::getReadSecond() {
  return taskInf_.curSec;
}

size_t conv::MixConverter::getWriteSecond() {
  return taskInf_.curSec;
}

bool conv::MixConverter::taskFinished() {
  return taskInf_.taskFinished;
}

size_t conv::MixConverter::getReadStream() {
  return taskInf_.stream;
}
/*
size_t conv::MixConverter::getWriteStream() {
  return taskInf_.stream;
}*/
