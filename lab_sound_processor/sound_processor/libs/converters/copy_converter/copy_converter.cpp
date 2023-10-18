#include "copy_converter.hpp"

namespace conv {
  void conv::CopyConverter::process(std::vector<int16_t>& mainSample,
                                    const std::vector<int16_t>& kSubSample) {
    if( taskInf_.curTime < (taskInf_.startTime + taskInf_.endTime) ) {
      size_t acceleratedSampleLen = mainSample.size() / acceleration_;
      for( size_t i = 0; i < acceleratedSampleLen; ++i ) {
        mainSample[(taskInf_.curTime - taskInf_.startTime) % acceleration_ * acceleratedSampleLen
                   + i] = kSubSample[i * acceleration_];
      }
    }
    ++taskInf_.curTime;
    taskInf_.taskFinished = taskInf_.curTime >= (taskInf_.startTime + taskInf_.endTime);
  }

  size_t conv::CopyConverter::getWriteSecond() {
    return taskInf_.startTime + (taskInf_.curTime - taskInf_.startTime) / acceleration_;
  }

  void conv::CopyConverter::setParams(const std::vector<size_t>& kParams) {
    Converter::setParams(kParams);
    acceleration_ = (taskInf_.otherParams.empty() || taskInf_.otherParams[0] == SIZE_MAX)
                          ? 1
                          : taskInf_.otherParams.front();
  }

  size_t conv::CopyConverter::getReadStream() {
    return ((taskInf_.curTime - taskInf_.startTime) % acceleration_) ? 0 : taskInf_.stream;
  }

  CopyConverter::CopyConverter(const std::vector<size_t>& kParams) {
    CopyConverter::setParams(kParams);
  }
}// namespace conv
