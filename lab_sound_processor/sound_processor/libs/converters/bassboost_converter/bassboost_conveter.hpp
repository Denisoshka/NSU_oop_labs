#pragma once

#include "converter.hpp"

namespace conv {
  class BassBoostConverter: public conv::Converter {
  public:
    BassBoostConverter() = default;
    BassBoostConverter(std::vector<size_t> params);
    ~BassBoostConverter() override = default;
    void process(std::vector<int16_t>& mainSample, std::vector<int16_t>& subSample) override;
    size_t getReadSecond() override;
    size_t getWriteSecond() override;
    void setParams(std::vector<size_t>&& params) override;
    bool taskFinished() override;
    size_t getReadStream() override;

  private:
    size_t BassBoostCoeficent_ = 1;
    int16_t bassFactor_ = 1000;
  };
}// namespace conv