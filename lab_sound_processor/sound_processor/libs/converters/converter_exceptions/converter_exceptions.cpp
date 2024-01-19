#include "converters.hpp"

conv::ConvertersException::ConvertersException(ConvertersExceptionsCode ErrorCode)
    : ErrorCode_(ErrorCode) {
}

conv::ConvertersExceptionsCode conv::ConvertersException::getErrorCode() const {
  return ErrorCode_;
}

[[nodiscard]] const char *conv::UnknownConverter::what() const noexcept{
  return std::invalid_argument::what();
}


conv::UnknownConverter::UnknownConverter(const std::string& kConverter)
    : std::invalid_argument(kConverter)
    , ConvertersException(ekUnknownConverter) {
}
