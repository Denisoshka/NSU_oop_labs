#pragma once
#include "converters.hpp"

namespace conv {
  class MuteConverter: public conv::Converter {
  public:
    MuteConverter() = default;
    MuteConverter(const std::vector<size_t>& kParams);
    virtual ~MuteConverter() override = default;
    virtual void process(std::vector<int16_t>& MainSample, const std::vector<int16_t>& kSubSample) override;
    virtual void setParams(const std::vector<size_t>& kParams) override;

  private:
  };
}// namespace conv
