#include "copy_converter.hpp"

namespace conv {
  void conv::CopyConverter::process(std::vector<int16_t>& MainSample,
                                    const std::vector<int16_t>& kSubSample) {
    if( TaskInf_.CurTime < (TaskInf_.StartTime + TaskInf_.EndTime) ) {
      size_t acceleratedSampleLen = MainSample.size() / acceleration_;
      for( size_t i = 0; i < acceleratedSampleLen; ++i ) {
        MainSample[(TaskInf_.CurTime - TaskInf_.StartTime) % acceleration_ * acceleratedSampleLen
                   + i] = kSubSample[i * acceleration_];
      }
    }
    ++TaskInf_.CurTime;
    TaskInf_.TaskFinished = TaskInf_.CurTime >= (TaskInf_.StartTime + TaskInf_.EndTime);
  }

  size_t conv::CopyConverter::getWriteSecond() {
    return TaskInf_.StartTime + (TaskInf_.CurTime - TaskInf_.StartTime) / acceleration_;
  }

  void conv::CopyConverter::setParams(const std::vector<size_t>& kParams) {
    Converter::setParams(kParams);
    acceleration_ = (TaskInf_.OtherParams.empty() || TaskInf_.OtherParams[0] == SIZE_MAX)
                          ? 1
                          : TaskInf_.OtherParams.front();
  }

  size_t conv::CopyConverter::getReadStream() {
    return ((TaskInf_.CurTime - TaskInf_.StartTime) % acceleration_) ? 0 : TaskInf_.Stream;
  }

  CopyConverter::CopyConverter(const std::vector<size_t>& kParams) {
    CopyConverter::setParams(kParams);
  }
}// namespace conv
