#pragma once

#include <ios>
#include <stdexcept>

namespace {

}// namespace

namespace conv {
  enum ConvertersExceptionsCode {
    ekUnknownConverter= 0x43'4F4'E56'1,
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
}// namespace conv
