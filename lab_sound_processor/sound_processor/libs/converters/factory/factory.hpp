#pragma once

#include <memory>

#include "converter.hpp"

namespace conv {
  std::unique_ptr<conv::Converter> makeConverter(const std::string& converterName,
                                                 std::vector<size_t>&& params);
}