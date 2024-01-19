#include "lowpass_converter.hpp"
#include "converters.hpp"

#include <cmath>

const size_t kNumTapsQuantity{200};
const size_t kFrequency{2000};

void conv::LowPassConverter::initLowPassFilter(const size_t kCutoffFrequency,
                                               const size_t kSampleRate) {
  double OmegaC =
          2.0 * M_PI * static_cast<double>(kCutoffFrequency) / static_cast<double>(kSampleRate);
  double Alpha = (NumTaps - 1) / 2.0;

  for( size_t i = 0; i < NumTaps; ++i ) {
    if( i == static_cast<size_t>(Alpha) ) {
      Coefficients[i] = OmegaC / M_PI;
    }
    else {
      Coefficients[i] = sin(OmegaC * (static_cast<double>(i) - Alpha))
                      / (M_PI * (static_cast<double>(i) - Alpha));
    }
  }
}

int16_t conv::LowPassConverter::applyLowPassFilter(const int16_t kSample) {
  Buffer.push_back(kSample);
  Buffer.erase(Buffer.begin());
  double output = 0.0;
  for( int j = 0; j < Buffer.size(); ++j ) {
    output += Buffer[j] * Coefficients[j];
  }
  return static_cast<int16_t>(output);
}

void conv::LowPassConverter::process(std::vector<int16_t>& MainSample,
                                     const std::vector<int16_t>& kSubSample) {
  if( TaskInf_.CurTime < TaskInf_.EndTime ) {
    auto & curSample = (InStream_) ? kSubSample :MainSample ;
    for( size_t i = 0; i < curSample.size(); ++i ) {
      MainSample[i] = applyLowPassFilter(curSample[i]);
    }
  }
  TaskInf_.CurTime++;
  TaskInf_.TaskFinished = TaskInf_.CurTime >= TaskInf_.EndTime || TaskInf_.CurTime >= TaskInf_.InDuration;
}

const int otherParamsStart = 2;
void conv::LowPassConverter::setParams(const std::vector<size_t>& kInStreams,
                                       const std::vector<size_t>& kParams) {
  Converter::setParams(kInStreams, kParams);
  Frequency_ = (kParams.size() < otherParamsStart  || kParams[otherParamsStart] == SIZE_MAX)
                     ? Frequency_
                     : kParams[otherParamsStart];
  initLowPassFilter(Frequency_);
}

conv::LowPassConverter::LowPassConverter()
    : NumTaps(kNumTapsQuantity)
    , Frequency_(kFrequency) {
  Coefficients.resize(kNumTapsQuantity);
  Buffer.resize(kNumTapsQuantity, 0.0);
}

conv::LowPassConverter::LowPassConverter(const std::vector<size_t>& kInStreams,
                                         const std::vector<size_t>& kParams)
    : LowPassConverter() {
  LowPassConverter::setParams(kInStreams, kParams);
}
