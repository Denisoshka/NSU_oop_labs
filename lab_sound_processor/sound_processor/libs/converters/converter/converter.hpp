#pragma once
#include <cstdint>
#include <vector>

namespace conv {
  class Converter {
  public:
    Converter() = default;
    Converter(const std::vector<size_t>& kParams);
    virtual ~Converter() = default;
    virtual void process(std::vector<int16_t>& MainSample,
                         const std::vector<int16_t>& kSubSample) = 0;
    virtual void setParams(const std::vector<size_t>& kParams);
    virtual size_t getReadSecond();
    virtual size_t getWriteSecond();
    [[nodiscard]] virtual bool taskFinished();
    virtual size_t getReadStream();

  protected:
    struct TaskParams {
      std::vector<size_t> OtherParams{};
      size_t Stream = 0;
      size_t StartTime = 0;
      size_t CurTime = 0;
      size_t EndTime = SIZE_MAX;
      bool TaskFinished = false;
    } TaskInf_;
  };

}// namespace conv