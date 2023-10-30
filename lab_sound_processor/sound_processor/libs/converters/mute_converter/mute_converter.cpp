#include "mute_converter.hpp"
#include "converters.hpp"

void conv::MuteConverter::process(std::vector<int16_t>& MainSample,
                                  const std::vector<int16_t>& kSubSample) {
  if( InStream_ ) {
    std::copy(kSubSample.begin(), kSubSample.end(), MainSample.begin());
  }
  if( TaskInf_.StartTime <= TaskInf_.CurTime && TaskInf_.CurTime < TaskInf_.EndTime ) {
    std::fill(MainSample.begin(), MainSample.end(), 0);
  }
  ++TaskInf_.CurTime;
  TaskInf_.TaskFinished = TaskInf_.CurTime >= TaskInf_.InDuration;
}

void conv::MuteConverter::setParams(const std::vector<size_t>& kInStreams,
                                    const std::vector<size_t>& kParams) {
  Converter::setParams(kInStreams, kParams);
  TaskInf_.CurTime = 0;
}

conv::MuteConverter::MuteConverter(const std::vector<size_t>& kInStreams,
                                   const std::vector<size_t>& kParams) {
  MuteConverter::setParams(kInStreams, kParams);
}
