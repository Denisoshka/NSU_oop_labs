#pragma once
#include "converter.hpp"

namespace conv {
  class LowPassConverter: public Converter {
  public:
    LowPassConverter();
    LowPassConverter(std::vector<size_t>&& params);
    ~LowPassConverter() override = default;
    void process(std::vector<int16_t>& mainSample, std::vector<int16_t>& subSample) override;
    void setParams(std::vector<size_t>&& params) override;

  private:
    void initLowPassFilter(size_t cutoffFrequency, size_t sampleRate = 44100);
    int16_t applyLowPassFilter(int16_t sample);
    std::vector<double> coefficients;
    std::vector<double> buffer;
    size_t frequency_;
    int num_taps;
  };
}
