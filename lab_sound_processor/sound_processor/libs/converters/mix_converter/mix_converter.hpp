#pragma once

#include "converter.hpp"

namespace conv {
  class MixConverter: public conv::Converter {
  public:
    MixConverter() = default;
    MixConverter(const std::vector<size_t>& kParams);
    virtual ~MixConverter() override = default;
    virtual void process(std::vector<int16_t>& mainSample, const std::vector<int16_t>& kSubSample) override;

  private:
  };
}// namespace conv