#pragma once

#include "converter.hpp"

namespace conv {
  class MixConverter: public conv::Converter {
  public:
    MixConverter() = default;
    MixConverter(std::vector<size_t>&& params);
    ~MixConverter() override = default;
    void process(std::vector<int16_t>& mainSample, std::vector<int16_t>& subSample) override;
    size_t getReadStream() override;

  private:
  };
}// namespace conv