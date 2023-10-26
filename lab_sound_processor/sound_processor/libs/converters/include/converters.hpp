#pragma once

#include <cstdint>
#include <ios>
#include <memory>
#include <stdexcept>
#include <vector>

namespace conv {
  enum ConvertersExceptionsCode {
    ekUnknownConverter = 0x43'4F4'E56'1,
  };

  class ConvertersException {
  public:
    ConvertersException(ConvertersExceptionsCode ErrorCode);
    [[nodiscard]] ConvertersExceptionsCode getErrorCode() const;
    [[nodiscard]] virtual const char *what() const = 0;

  protected:
    ConvertersExceptionsCode ErrorCode_;
  };

  class UnknownConverter: public std::invalid_argument,
                          virtual public ConvertersException {
  public:
    explicit UnknownConverter(const std::string& kConverter);
    [[nodiscard]] virtual const char *what() const noexcept override;
  };

  class Converter {
  public:
    Converter() = default;
    Converter(const std::vector<size_t>& kInStreams, const std::vector<size_t>& kParams);
    virtual ~Converter() = default;
    virtual void process(std::vector<int16_t>& MainSample,
                         const std::vector<int16_t>& kSubSample) = 0;
    virtual void setParams(const std::vector<size_t>& kInStreams, const std::vector<size_t>& kParams);
    virtual size_t getReadSecond();
    virtual size_t getWriteSecond();
    [[nodiscard]] virtual bool taskFinished();
    [[nodiscard]] virtual size_t inStreamQuantity() const ;
    virtual void setInDuration(const size_t InDuration);
    virtual size_t getReadStream();

  protected:
    struct TaskParams {
      size_t InDuration = SIZE_MAX;
      size_t StartTime = 0;
      size_t CurTime = 0;
      size_t EndTime = SIZE_MAX;
      bool TaskFinished = false;
    } TaskInf_;
    size_t InStream_;

  };

  std::unique_ptr<conv::Converter> makeConverter(const std::string& kConverterName,
                                                 const std::vector<size_t>& kInStreams,
                                                 const std::vector<size_t>& kParams);
  std::string GetConvertersDescription();

}// namespace conv
