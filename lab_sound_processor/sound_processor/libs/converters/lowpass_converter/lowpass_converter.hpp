#pragma once
#include "converters.hpp"

namespace conv {
  class LowPassConverter: public Converter {
  public:
    LowPassConverter();
    LowPassConverter(const std::vector<size_t>& kInStreams, const std::vector<size_t>& kParams);
    virtual ~LowPassConverter() override = default;
    virtual void process(std::vector<int16_t>& MainSample, const std::vector<int16_t>& kSubSample) override;
    virtual void setParams(const std::vector<size_t>& kInStreams,
                           const std::vector<size_t>& kParams) override;

  private:
    void initLowPassFilter(const size_t kCutoffFrequency, const size_t kSampleRate = 44100);
    int16_t applyLowPassFilter(const int16_t kSample);
    std::vector<double> Coefficients;
    std::vector<double> Buffer;
    size_t Frequency_;
    int NumTaps;
  };
}
