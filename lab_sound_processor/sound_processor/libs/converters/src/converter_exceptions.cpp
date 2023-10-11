#include "converter_exceptions.hpp"


conv::StreamFailure::StreamFailure(const std::string& filePath)
    : std::ios_base::failure(filePath + " failure ") {
}

conv::IncorrectSettingsFormat::IncorrectSettingsFormat(const std::string& settings)
    : std::invalid_argument(settings) {
}

conv::UnckonwConverter::UnckonwConverter(const std::string& converter)
    : std::invalid_argument(converter) {
}