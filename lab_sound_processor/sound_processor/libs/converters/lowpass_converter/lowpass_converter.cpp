#include "lowpass_converter.hpp"
#include "converters.hpp"

#include <cmath>

void conv::LowPassConverter::initLowPassFilter(const size_t kCutoffFrequency, const size_t kSampleRate) {
  double omega_c =
          2.0 * M_PI * static_cast<double>(kCutoffFrequency) / static_cast<double>(kSampleRate);
  double alpha = (num_taps - 1) / 2.0;

  for( size_t i = 0; i < num_taps; ++i ) {
    if( i == static_cast<size_t>(alpha) ) {
      coefficients[i] = omega_c / M_PI;
    }
    else {
      coefficients[i] = sin(omega_c * (static_cast<double>(i) - alpha))
                      / (M_PI * (static_cast<double>(i) - alpha));
    }
  }
}

int16_t conv::LowPassConverter::applyLowPassFilter(const int16_t kSample) {
  buffer.push_back(kSample);
  buffer.erase(buffer.begin());
  double output = 0.0;
  for( int j = 0; j < buffer.size(); ++j ) {
    output += buffer[j] * coefficients[j];
  }
  return static_cast<int16_t>(output);
}

void conv::LowPassConverter::process(std::vector<int16_t>& mainSample,
                                     const std::vector<int16_t>& kSubSample) {
  if( taskInf_.curTime < taskInf_.endTime ) {
    for( size_t i = 0; i < kSubSample.size(); ++i ) {
      mainSample[i] = applyLowPassFilter(mainSample[i]);
    }
  }
  taskInf_.curTime++;
  taskInf_.taskFinished = taskInf_.curTime >= taskInf_.endTime;
}

void conv::LowPassConverter::setParams(const std::vector<size_t>& kParams) {
  Converter::setParams(kParams);
  frequency_ = (taskInf_.otherParams.empty() || taskInf_.otherParams.front() == SIZE_MAX)
                     ? frequency_
                     : taskInf_.otherParams.front();
  initLowPassFilter(frequency_);
}

conv::LowPassConverter::LowPassConverter()
    : num_taps(200)
    , frequency_(2000) {
  coefficients.resize(num_taps);
  buffer.resize(num_taps, 0.0);
}

conv::LowPassConverter::LowPassConverter(const std::vector<size_t>& kParams)
    : LowPassConverter() {
  LowPassConverter::setParams(kParams);
}
