#pragma once

#include <ios>
#include <stdexcept>

namespace conv {
  class UnckownConverter: public std::invalid_argument {
  public:
    explicit UnckownConverter(const std::string& kConverter);
  };
}// namespace conv
