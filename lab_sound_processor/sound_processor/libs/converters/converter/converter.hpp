#pragma once
#include <cstdint>
#include <memory>
#include <vector>

namespace conv {
  struct TaskParams {
    std::vector<size_t> otherParams{};
    size_t stream = 0;
    size_t startTime = 0;
    size_t curTime = 0;
    size_t endTime = SIZE_MAX;
    bool taskFinished = false;
  };

  TaskParams convertToCONVParams(std::vector<size_t>&& params);

  class Converter {
  public:
    Converter() = default;
    Converter(std::vector<size_t>&& params);
    virtual ~Converter() = default;
    virtual void process(std::vector<int16_t>& mainSample, std::vector<int16_t>& subSample) = 0;
    virtual void setParams(std::vector<size_t>&& params);
    virtual size_t getReadSecond();
    virtual size_t getWriteSecond();
    [[nodiscard]] virtual bool taskFinished();
    virtual size_t getReadStream();

  protected:
    TaskParams taskInf_;
  };

}// namespace conv