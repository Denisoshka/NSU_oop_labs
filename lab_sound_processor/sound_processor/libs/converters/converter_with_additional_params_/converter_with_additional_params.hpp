#pragma once
#include "converters.hpp"
#include "version"

namespace conv {
  class ConverterWithAdditionalParams: public conv::Converter {
  public:
    virtual void setParams(const std::vector<size_t>& kInStreams,
                           const std::vector<size_t>& kParams) override;

  protected:
    std::vector<size_t> OtherParams{};
  };
}// namespace conv
