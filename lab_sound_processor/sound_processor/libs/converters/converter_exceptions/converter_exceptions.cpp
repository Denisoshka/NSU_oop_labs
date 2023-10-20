#include "converter_exceptions.hpp"

conv::UnckownConverter::UnckownConverter(const std::string& kConverter)
    : std::invalid_argument(kConverter) {
}