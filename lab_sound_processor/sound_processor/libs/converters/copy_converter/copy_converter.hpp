#pragma once

#include "converter.hpp"

namespace conv {
  class CopyConverter: public Converter {
  public:
    CopyConverter() = default;
    ~CopyConverter() override = default;
    void process(std::vector<int16_t>& mainSample, std::vector<int16_t>& subSample) override;
    size_t getWriteSecond() override;
    void setParams(conv::TaskParams&& params) override;
    size_t getReadStream() override;

  private:
    size_t acceleration_ = 1;
  };
}// namespace conv