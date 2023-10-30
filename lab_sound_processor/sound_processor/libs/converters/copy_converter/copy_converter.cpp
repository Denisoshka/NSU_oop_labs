#include "copy_converter.hpp"

namespace conv {
  void conv::CopyConverter::process(std::vector<int16_t>& MainSample,
                                    const std::vector<int16_t>& kSubSample) {
    if( TaskInf_.CurTime < (TaskInf_.StartTime + TaskInf_.EndTime) ) {
      auto& curSample = (InStream_) ? kSubSample : MainSample;
      size_t acceleratedSampleLen = MainSample.size() / acceleration_;
      for( size_t i = 0; i < acceleratedSampleLen; ++i ) {
        MainSample[(TaskInf_.CurTime - TaskInf_.StartTime) % acceleration_ * acceleratedSampleLen
                   + i] = curSample[i * acceleration_];
      }
    }
    ++TaskInf_.CurTime;
    TaskInf_.TaskFinished = TaskInf_.CurTime >= (TaskInf_.StartTime + TaskInf_.EndTime)
                         || TaskInf_.CurTime >= TaskInf_.InDuration;
  }

  size_t conv::CopyConverter::getWriteSecond() {
    return TaskInf_.StartTime + (TaskInf_.CurTime - TaskInf_.StartTime) / acceleration_;
  }

  size_t kOtherParamsStart = 2;

  void conv::CopyConverter::setParams(const std::vector<size_t>& kInStreams,
                                      const std::vector<size_t>& kParams) {
    Converter::setParams(kInStreams, kParams);
    acceleration_ = (kParams.size() > kOtherParamsStart && kParams[2] != SIZE_MAX) ? kParams[2] : 1;
  }

  size_t conv::CopyConverter::getReadStream() {
    return ((TaskInf_.CurTime - TaskInf_.StartTime) % acceleration_) ? 0 : InStream_;
  }

  CopyConverter::CopyConverter(const std::vector<size_t>& kInStreams,
                               const std::vector<size_t>& kParams) {
    CopyConverter::setParams(kInStreams, kParams);
  }
}// namespace conv
