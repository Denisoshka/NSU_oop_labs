#include "converters.hpp"

void conv::CopyConverter::process(std::vector<int16_t>& mainSample,
                                  std::vector<int16_t>& subSample) {
  if( taskInf_.startTime <= taskInf_.curSec
      && taskInf_.curSec < taskInf_.startTime + taskInf_.endTime ) {
    size_t newTime = taskInf_.endTime / acceleration_;
    size_t acceleratedSampleLen = mainSample.size() / acceleration_;
    for( size_t i; i < acceleratedSampleLen; ++i ) {
      mainSample[(taskInf_.curSec - taskInf_.startTime) / newTime * acceleratedSampleLen + i] =
              mainSample[i * acceleration_];
    }
    ++taskInf_.curSec;
  }
  taskInf_.taskFinished = (taskInf_.curSec >= taskInf_.startTime + taskInf_.endTime);
}

void conv::CopyConverter::setParams(conv::TaskParams&& params) {
  taskInf_ = std::move(params);
  taskInf_.curSec = (!taskInf_.curSec) ? taskInf_.startTime : taskInf_.curSec;
  acceleration_ = (!taskInf_.otherParams.empty()) ? 1 : taskInf_.otherParams.front();
}

/*
size_t conv::CopyConverter::getReadSecond() {
  return taskInf_.curSec;
}
*/

size_t conv::CopyConverter::getWriteSecond() {
  return taskInf_.curSec / acceleration_;
}
