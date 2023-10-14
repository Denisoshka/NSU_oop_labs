#include <cmath>
#include "converters.hpp"
/*
double countMean(const std::vector<int16_t>& sample) {
  double mean = 0;
  for( double point: sample ) {
    mean += point;
  }
  return mean / static_cast<double>(sample.size());
}

double countStd(const std::vector<int16_t>& sample, double mean) {
  double std = 0;
  for( double point: sample ) {
    std += (point - mean) * (point - mean);
  }
  std = 3 * std::sqrt(std / static_cast<double>(2 * sample.size()));
  return std;
}

uint16_t bass_line_freq() {
}
*/


void conv::LowPassConverter::initLowPassFilter(size_t cutoffFrequency, size_t sampleRate) {
  // Рассчитываем коэффициенты для FIR-фильтра
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
  //  double mean = countMean(subSample);
  //  double std = countStd(subSample, mean);
  //  uint16_t bassFactor = static_cast<int16_t>(round(std - mean) * 0.005);
  if( taskInf_.curSec < taskInf_.endTime ) {
    for( size_t i = 0; i < subSample.size(); ++i ) {
      mainSample[i] = applyLowPassFilter(mainSample[i]);
    }
  }
  taskInf_.curSec++;
  taskInf_.taskFinished = taskInf_.curSec >= taskInf_.endTime;
}

void conv::LowPassConverter::setParams(conv::TaskParams& params) {
  taskInf_ = params;
  taskInf_.curSec = taskInf_.startTime;
  if( !params.otherParams.empty() ) {
    frequency = params.otherParams.front();
  }
  initLowPassFilter(frequency);
}

size_t conv::LowPassConverter::getReadSecond() {
  return taskInf_.curSec;
}

size_t conv::LowPassConverter::getWriteSecond() {
  return taskInf_.curSec;
}

bool conv::LowPassConverter::taskFinished() {
  return taskInf_.taskFinished;
}

size_t conv::LowPassConverter::getReadStream() {
  return taskInf_.stream;
}

conv::LowPassConverter::LowPassConverter()
    : num_taps(200) {
  coefficients.resize(num_taps);
  buffer.resize(num_taps, 0.0);
}