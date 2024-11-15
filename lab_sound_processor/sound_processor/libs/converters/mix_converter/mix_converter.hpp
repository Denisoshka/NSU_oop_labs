#pragma once

#include "converters.hpp"

namespace conv {
  class MixConverter: public conv::Converter {
  public:
    MixConverter() = default;
    MixConverter(const std::vector<size_t>& kInStreams, const std::vector<size_t>& kParams);
    virtual ~MixConverter() override = default;
    virtual void process(std::vector<int16_t>& MainSample,
                         const std::vector<int16_t>& kSubSample) override;
    virtual void setParams(const std::vector<size_t>& kInStreams,
                           const std::vector<size_t>& kParams) override;
    [[nodiscard]] virtual bool taskFinished() override;
    [[nodiscard]] virtual size_t inStreamQuantity() const override ;
    virtual size_t getReadStream() override;
  protected:
    std::vector<size_t> InStreams_{};
    int StreamsQuantity;
  };
}// namespace conv