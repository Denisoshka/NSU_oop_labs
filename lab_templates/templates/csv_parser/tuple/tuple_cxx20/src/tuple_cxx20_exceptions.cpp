#include "tuple_cxx20_exceptions.hpp"

namespace tupleCXX20Exceptions {

  tupleCXX20ExceptionErrorsId baseTupleCXX20Exception::getId() {
    return id_;
  }

  baseTupleCXX20Exception::baseTupleCXX20Exception(tupleCXX20ExceptionErrorsId id)
      : id_(id) {
  }

  tupleCXX20InvalidConversion::tupleCXX20InvalidConversion(const std::string& kMsg,
                                                           const size_t kColum)
      : baseTupleCXX20Exception(tupleCXX20ExceptionErrorsId::ekTupleCXX20InvalidConversion)
      , kColum_(kColum)
      , invalid_argument(kMsg) {
  }

  size_t tupleCXX20InvalidConversion::getColum() const {
    return kColum_;
  }
}// namespace tupleCXX20Exceptions