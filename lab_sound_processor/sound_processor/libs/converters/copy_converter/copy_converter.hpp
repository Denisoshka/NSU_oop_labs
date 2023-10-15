#pragma once

#include "converter.hpp"

namespace conv {
  class CopyConverter: public Converter {
  public:
    CopyConverter() = default;
    CopyConverter(std::vector<size_t>&& params);
    ~CopyConverter() override = default;
    void process(std::vector<int16_t>& mainSample, std::vector<int16_t>& subSample) override;
    size_t getWriteSecond() override;
    void setParams(std::vector<size_t>&& params) override;
    size_t getReadStream() override;

  private:
    size_t acceleration_ = 1;
  };
}// namespace conv