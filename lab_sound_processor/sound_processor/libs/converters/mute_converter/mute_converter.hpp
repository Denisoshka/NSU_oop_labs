#pragma once
#include "converters.hpp"

namespace conv {
  class MuteConverter: public conv::Converter {
  public:
    MuteConverter() = default;
    MuteConverter(std::vector<size_t>&& params);
    ~MuteConverter() override = default;
    void process(std::vector<int16_t>& mainSample, std::vector<int16_t>& subSample) override;
    void setParams(std::vector<size_t>&& params) override;

  private:
  };
}// namespace conv
