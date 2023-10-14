#pragma once

#include <memory>

#include "converter.hpp"

namespace conv {
  std::unique_ptr<conv::Converter> makeConverter(const std::string& converterName,
                                                 TaskParams&& params);
}