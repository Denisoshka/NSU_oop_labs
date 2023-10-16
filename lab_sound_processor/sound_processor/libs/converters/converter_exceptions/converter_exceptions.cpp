#include "converter_exceptions.hpp"

conv::UnckonwConverter::UnckonwConverter(const std::string& converter)
    : std::invalid_argument(converter) {
}