#pragma once
#include <stdexcept>

namespace CSVParserExceptions {
  class CSVException: public std::runtime_error {
  public:
    CSVException(size_t row, size_t column, const std::string& kMsg)
        : std::runtime_error("(" + std::to_string(row) + ", " + std::to_string(column)
                             + ") : " + kMsg) {
    }
  };

  class FewColumnsException: public CSVException {
  public:
    FewColumnsException(size_t row, size_t column)
        : CSVException(row, column, "Few columns") {
    }
  };

  class TypeMismatchException: public CSVException {
  public:
    TypeMismatchException(size_t row, size_t column, const std::string& kDataType)
        : CSVException(row, column, "Invalid conversion to " + kDataType) {
    }
  };

  class FewRowsException: public CSVException {
  public:
    FewRowsException(const size_t quantity, const std::string& kMsg)
        : CSVException(quantity, 0, "Few Rows Quantity: " + std::to_string(quantity) + " " + kMsg) {
    }
  };
}// namespace CSVParserExceptions