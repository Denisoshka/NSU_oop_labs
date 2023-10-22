#pragma once

#include <ios>
#include <stdexcept>

namespace process {
  enum ProcessExceptions : size_t{
    ekProcessExceptionStd = 999'0,
    ekStreamFailure,
    ekIncorrectSettingsFormat,
  };

  class ProcessException {
  public:
    explicit ProcessException(ProcessExceptions code);
    [[nodiscard]] ProcessExceptions getErrorCode() const;
    [[nodiscard]] virtual const char *what() const noexcept = 0;

  protected:
    ProcessExceptions ErrorCode_;
  };

  class StreamFailure: virtual public ProcessException,
                       public std::ios_base::failure {
  public:
    explicit StreamFailure(const std::string& kFilePath);
    [[nodiscard]] virtual const char *what() const noexcept override;
  };

  class IncorrectSettingsFormat: virtual public ProcessException,
                                 public std::invalid_argument {
  public:
    explicit IncorrectSettingsFormat(const std::string& kSettings);
    [[nodiscard]] virtual const char *what() const noexcept override;
  };
}// namespace process
