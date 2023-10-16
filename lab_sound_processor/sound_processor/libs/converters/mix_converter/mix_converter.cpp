#include "mix_converter.hpp"
#include "converter.hpp"

void conv::MixConverter::process(std::vector<int16_t>& mainSample,
                                 const std::vector<int16_t>& kSubSample) {
  if( taskInf_.curTime < taskInf_.endTime ) {
    for( size_t i = 0; i < mainSample.size(); ++i ) {
      mainSample[i] = mainSample[i] / 2 + kSubSample[i] / 2;
    }
  }
  taskInf_.curTime++;
  taskInf_.taskFinished = taskInf_.curTime >= taskInf_.endTime;
}

conv::MixConverter::MixConverter(const std::vector<size_t>& kParams)
    : Converter(std::move(kParams)) {
}
