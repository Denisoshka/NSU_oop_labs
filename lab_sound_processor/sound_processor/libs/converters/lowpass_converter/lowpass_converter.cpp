#include "converters.hpp"
#include "lowpass_converter.hpp"

#include <cmath>

void conv::LowPassConverter::initLowPassFilter(size_t cutoffFrequency, size_t sampleRate) {
  double omega_c =
          2.0 * M_PI * static_cast<double>(cutoffFrequency) / static_cast<double>(sampleRate);
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

int16_t conv::LowPassConverter::applyLowPassFilter(int16_t sample) {
  buffer.push_back(sample);
  buffer.erase(buffer.begin());
  double output = 0.0;
  for( int j = 0; j < buffer.size(); ++j ) {
    output += buffer[j] * coefficients[j];
  }
  return static_cast<int16_t>(output);
}

void conv::LowPassConverter::process(std::vector<int16_t>& mainSample,
                                     std::vector<int16_t>& subSample) {
  if( taskInf_.curTime < taskInf_.endTime ) {
    for( size_t i = 0; i < subSample.size(); ++i ) {
      mainSample[i] = applyLowPassFilter(mainSample[i]);
    }
  }
  taskInf_.curTime++;
  taskInf_.taskFinished = taskInf_.curTime >= taskInf_.endTime;
}

void conv::LowPassConverter::setParams(conv::TaskParams&& params) {
  taskInf_ = params;
  taskInf_.curTime = taskInf_.startTime;
  if( !params.otherParams.empty() ) {
    frequency = params.otherParams.front();
  }
  initLowPassFilter(frequency);
}

conv::LowPassConverter::LowPassConverter()
    : num_taps(200) {
  coefficients.resize(num_taps);
  buffer.resize(num_taps, 0.0);
}