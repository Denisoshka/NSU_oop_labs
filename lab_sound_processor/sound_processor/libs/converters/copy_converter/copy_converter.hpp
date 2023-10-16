#pragma once

#include "converter.hpp"

namespace conv {
  class CopyConverter: public Converter {
  public:
    CopyConverter() = default;
    CopyConverter(const std::vector<size_t>& params);
    virtual ~CopyConverter() override = default;
    virtual void process(std::vector<int16_t>& mainSample,
                         const std::vector<int16_t>& kSubSample) override;
    virtual size_t getWriteSecond() override;
    virtual void setParams(const std::vector<size_t>& kParams) override;
    virtual size_t getReadStream() override;

  private:
    size_t acceleration_ = 1;
  };
}// namespace conv