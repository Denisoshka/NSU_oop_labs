#pragma once

#include <stdexcept>

namespace tupleCXX20Exceptions {
  enum tupleCXX20ExceptionErrorsId : size_t {
    ekTupleCXX20ExceptionError = 11611711210810167'0,
    ekTupleCXX20InvalidConversion,
  };

  class baseTupleCXX20Exception {
  public:
    explicit baseTupleCXX20Exception(tupleCXX20ExceptionErrorsId id);
    tupleCXX20ExceptionErrorsId getId();

  protected:
    tupleCXX20ExceptionErrorsId id_;
  };

  class tupleCXX20InvalidConversion: public virtual baseTupleCXX20Exception,
                                     public std::invalid_argument {
  public:
    tupleCXX20InvalidConversion(const std::string& kMsg, const size_t kColum);
    size_t getColum() const;
  private:
    size_t kColum_;
  };
}// namespace tupleCXX20Exceptions