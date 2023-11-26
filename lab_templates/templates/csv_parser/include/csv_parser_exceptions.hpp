#pragma once
#include <stdexcept>
namespace CSVParserExceptions {
  class CSVException: public std::runtime_error {
  public:
    CSVException(size_t row, size_t column, const std::string& kMsg);
  };

  class FewColumnsException: public CSVException {
  public:
    FewColumnsException(size_t row, size_t column);
  };

  class TypeMismatchException: public CSVException {
  public:
    TypeMismatchException(size_t row, size_t column, const std::string& kDataType);
  };

  class FewRowsException :public CSVException {
  public:
    FewRowsException(const size_t quantity, const std::string& kMsg);
  };
}// namespace CSVParserExceptions