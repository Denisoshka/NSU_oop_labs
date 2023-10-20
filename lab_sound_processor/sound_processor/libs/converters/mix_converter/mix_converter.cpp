#include "mix_converter.hpp"
#include "converter.hpp"

void conv::MixConverter::process(std::vector<int16_t>& MainSample,
                                 const std::vector<int16_t>& kSubSample) {
  if( TaskInf_.CurTime < TaskInf_.EndTime ) {
    for( size_t i = 0; i < MainSample.size(); ++i ) {
      MainSample[i] = MainSample[i] / 2 + kSubSample[i] / 2;
    }
  }
  TaskInf_.CurTime++;
  TaskInf_.TaskFinished = TaskInf_.CurTime >= TaskInf_.EndTime;
}

conv::MixConverter::MixConverter(const std::vector<size_t>& kParams)
    : Converter(kParams) {
}
