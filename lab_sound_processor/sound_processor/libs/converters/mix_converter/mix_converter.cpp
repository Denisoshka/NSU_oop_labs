#include "mix_converter.hpp"
#include "converter.hpp"

void conv::MixConverter::process(std::vector<int16_t>& mainSample,
                                 std::vector<int16_t>& subSample) {
  if( taskInf_.curTime < taskInf_.endTime ) {
    for( size_t i = 0; i < mainSample.size(); ++i ) {
      mainSample[i] = mainSample[i] / 2 + subSample[i] / 2;
    }
  }
  taskInf_.curTime++;
  taskInf_.taskFinished = taskInf_.curTime >= taskInf_.endTime;
}

size_t conv::MixConverter::getReadStream() {
  return 0;
}

conv::MixConverter::MixConverter(conv::TaskParams&& params)
    : Converter(std::move(params)) {
}
