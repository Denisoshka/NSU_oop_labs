#pragma once

#include "converter.hpp"

namespace conv {
  class BassBoostConverter: public conv::Converter {
  public:
    BassBoostConverter() = default;
    BassBoostConverter(const std::vector<size_t>& kParams);
    virtual ~BassBoostConverter() override = default;
    virtual void process(std::vector<int16_t>& mainSample, const std::vector<int16_t>& kSubSample) override;
    virtual void setParams(const std::vector<size_t>& kParams) override;

  private:
    size_t BassBoostCoeficent_ = 1;
    int16_t bassFactor_ = 1000;
  };
}// namespace conv
