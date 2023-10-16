#pragma once
#include <cstdint>
#include <vector>

namespace conv {
  class Converter {
  public:
    Converter() = default;
    Converter(const std::vector<size_t>& params);
    virtual ~Converter() = default;
    virtual void process(std::vector<int16_t>& mainSample,
                         const std::vector<int16_t>& kSubSample) = 0;
    virtual void setParams(const std::vector<size_t>& kParams);
    virtual size_t getReadSecond();
    virtual size_t getWriteSecond();
    [[nodiscard]] virtual bool taskFinished();
    virtual size_t getReadStream();

  protected:
    struct TaskParams {
      std::vector<size_t> otherParams{};
      size_t stream = 0;
      size_t startTime = 0;
      size_t curTime = 0;
      size_t endTime = SIZE_MAX;
      bool taskFinished = false;
    } taskInf_;
  };

}// namespace conv