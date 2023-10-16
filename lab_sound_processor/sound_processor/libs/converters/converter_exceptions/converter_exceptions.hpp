#pragma once

#include <ios>
#include <stdexcept>

namespace conv {
  class UnckonwConverter: public std::invalid_argument {
  public:
    explicit UnckonwConverter(const std::string& converter);
  };
}// namespace conv
