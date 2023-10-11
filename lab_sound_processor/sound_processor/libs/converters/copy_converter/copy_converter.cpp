#include "converters.hpp"

void conv::CopyConverter::process(std::vector<int16_t>& mainSample,
                                  std::vector<int16_t>& subSample) {
  if( taskInf_.curSec < (taskInf_.startTime + taskInf_.endTime) ) {
    size_t newTime = taskInf_.endTime / acceleration_;
    size_t acceleratedSampleLen = mainSample.size() / acceleration_;
    for( size_t i = 0; i < acceleratedSampleLen; ++i ) {
      mainSample[(taskInf_.curSec - taskInf_.startTime) % acceleration_ * acceleratedSampleLen
                 + i] = subSample[i * acceleration_];
    }
  }
  ++taskInf_.curSec;
  taskInf_.taskFinished = taskInf_.curSec >= (taskInf_.startTime + taskInf_.endTime);
}

size_t conv::CopyConverter::getWriteSecond() {
  return taskInf_.startTime + (taskInf_.curSec - taskInf_.startTime) / acceleration_;
}

size_t conv::CopyConverter::getReadSecond() {
  return taskInf_.curSec;
}

void conv::CopyConverter::setParams(conv::TaskParams& params) {
  taskInf_ = params;
  //  acceleration_ = taskInf_.endTime;
  taskInf_.curSec = taskInf_.startTime;
  acceleration_ = (taskInf_.otherParams.empty()) ? 1 : taskInf_.otherParams.front();
}

bool conv::CopyConverter::taskFinished() {
  return taskInf_.taskFinished;
}

size_t conv::CopyConverter::getReadStream() {
  return ((taskInf_.curSec - taskInf_.startTime) % acceleration_) ? 0 : taskInf_.stream;
}
/*
size_t conv::CopyConverter::getWriteStream() {
  return taskInf_.stream;
}*/
