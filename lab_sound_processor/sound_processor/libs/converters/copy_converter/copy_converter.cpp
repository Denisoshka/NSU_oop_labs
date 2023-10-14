#include "copy_converter.hpp"

namespace conv {
  void conv::CopyConverter::process(std::vector<int16_t>& mainSample,
                                    std::vector<int16_t>& subSample) {
    if( taskInf_.curTime < (taskInf_.startTime + taskInf_.endTime) ) {
      size_t newTime = taskInf_.endTime / acceleration_;
      size_t acceleratedSampleLen = mainSample.size() / acceleration_;
      for( size_t i = 0; i < acceleratedSampleLen; ++i ) {
        mainSample[(taskInf_.curTime - taskInf_.startTime) % acceleration_ * acceleratedSampleLen
                   + i] = subSample[i * acceleration_];
      }
    }
    ++taskInf_.curTime;
    taskInf_.taskFinished = taskInf_.curTime >= (taskInf_.startTime + taskInf_.endTime);
  }

  size_t conv::CopyConverter::getWriteSecond() {
    return taskInf_.startTime + (taskInf_.curTime - taskInf_.startTime) / acceleration_;
  }

  void conv::CopyConverter::setParams(conv::TaskParams&& params) {
    taskInf_ = params;
    taskInf_.curTime = taskInf_.startTime;
    acceleration_ = (taskInf_.otherParams.empty()) ? 1 : taskInf_.otherParams.front();
  }

  size_t conv::CopyConverter::getReadStream() {
    return ((taskInf_.curTime - taskInf_.startTime) % acceleration_) ? 0 : taskInf_.stream;
  }

}// namespace conv