#pragma once
#include "converters.hpp"

namespace conv {
  class MuteConverter: public conv::Converter {
  public:
    MuteConverter() = default;
    MuteConverter(TaskParams && params);
    ~MuteConverter() override = default;
    void process(std::vector<int16_t>& mainSample, std::vector<int16_t>& subSample) override;
    void setParams(conv::TaskParams&& params) override;

  private:
  };
}// namespace conv
