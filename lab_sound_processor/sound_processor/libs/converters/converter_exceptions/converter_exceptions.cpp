#include "converter_exceptions.hpp"

conv::UnckownConverter::UnckownConverter(const std::string& converter)
    : std::invalid_argument(converter) {
}