#include "csv_parser_exceptions.hpp"

namespace CSVParserExceptions {

  CSVException::CSVException(size_t row, size_t column, const std::string& kMsg)
      : std::runtime_error("(" + std::to_string(row) + ", " + std::to_string(column)
                           + ") : " + kMsg) {
  }

  FewColumnsException::FewColumnsException(size_t row, size_t column)
      : CSVException(row, column, "Few columns") {
  }

  TypeMismatchException::TypeMismatchException(size_t row, size_t column,
                                               const std::string& kDataType)
      : CSVException(row, column, "Invalid conversion to " + kDataType) {
  }

  FewRowsException::FewRowsException(const size_t quantity, const std::string& kMsg)
      : CSVException(quantity, 0, "Few Rows Quantity: " + std::to_string(quantity) + " " + kMsg) {
  }
}// namespace CSVParserExceptions