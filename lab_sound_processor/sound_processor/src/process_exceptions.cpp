#include "process_exceptions.hpp"

namespace process {
  ProcessException::ProcessException(ProcessExceptions code)
      : ErrorCode_(code) {
  }

  ProcessExceptions ProcessException::getErrorCode() const {
    return ErrorCode_;
  }

  StreamFailure::StreamFailure(const std::string& kFilePath)
      : ProcessException(ekStreamFailure)
      , std::ios_base::failure(kFilePath + " failure ") {
  }

  const char *StreamFailure::what() const noexcept {
    return std::ios_base::failure::what();
  }

  IncorrectSettingsFormat::IncorrectSettingsFormat(const std::string& kSettings)
      : ProcessException(ekIncorrectSettingsFormat)
      , std::invalid_argument(kSettings) {
  }

  const char * IncorrectSettingsFormat::what() const noexcept {
    return std::invalid_argument::what();
  }
}// namespace process