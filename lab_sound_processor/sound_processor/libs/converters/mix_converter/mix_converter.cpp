#include "mix_converter.hpp"
#include "converters.hpp"

void conv::MixConverter::process(std::vector<int16_t>& MainSample,
                                 const std::vector<int16_t>& kSubSample) {
  if( TaskInf_.CurTime < TaskInf_.EndTime ) {
    for( size_t i = 0; i < MainSample.size(); ++i ) {
      MainSample[i] = MainSample[i]/ InStreams_.size() + kSubSample[i] ;
    }
  }
  ++TaskInf_.CurTime;
  TaskInf_.TaskFinished =
          TaskInf_.CurTime >= TaskInf_.EndTime || TaskInf_.CurTime >= TaskInf_.InDuration;
}

conv::MixConverter::MixConverter(const std::vector<size_t>& kInStreams,
                                 const std::vector<size_t>& kParams) {
  MixConverter::setParams(kInStreams, kParams);
}

void conv::MixConverter::setParams(const std::vector<size_t>& kInStreams,
                                   const std::vector<size_t>& kParams) {
  Converter::setParams(kInStreams, kParams);
  for( const auto stream: kInStreams ) {
    InStreams_.push_back((stream == SIZE_MAX) ? 0 : stream);
  }
}

size_t conv::MixConverter::getReadStream() {
  return InStreams_.back();
}

bool conv::MixConverter::taskFinished() {
  bool x = TaskInf_.TaskFinished;
  if( TaskInf_.TaskFinished ) {
    InStreams_.pop_back();
    TaskInf_.CurTime = TaskInf_.StartTime;
    TaskInf_.TaskFinished = InStreams_.empty();
  }
  return x;
}

size_t conv::MixConverter::inStreamQuantity() const {
  return InStreams_.size();
}
