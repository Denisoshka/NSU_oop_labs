#include "lowpass_converter.hpp"
#include "converters.hpp"

#include <cmath>

void conv::LowPassConverter::initLowPassFilter(const size_t kCutoffFrequency, const size_t kSampleRate) {
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
    for( size_t i = 0; i < kSubSample.size(); ++i ) {
      MainSample[i] = applyLowPassFilter(MainSample[i]);
    }
  }
  TaskInf_.CurTime++;
  TaskInf_.TaskFinished = TaskInf_.CurTime >= TaskInf_.EndTime;
}

void conv::LowPassConverter::setParams(const std::vector<size_t>& kParams) {
  Converter::setParams(kParams);
  Frequency_ = (TaskInf_.OtherParams.empty() || TaskInf_.OtherParams.front() == SIZE_MAX)
                     ? Frequency_
                     : TaskInf_.OtherParams.front();
  initLowPassFilter(Frequency_);
}

conv::LowPassConverter::LowPassConverter()
    : NumTaps(200)
    , Frequency_(2000) {
  Coefficients.resize(NumTaps);
  Buffer.resize(NumTaps, 0.0);
}

conv::LowPassConverter::LowPassConverter(const std::vector<size_t>& kParams)
    : LowPassConverter() {
  LowPassConverter::setParams(kParams);
}
