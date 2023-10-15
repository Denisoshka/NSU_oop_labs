#include "process_exceptions.hpp"

StreamFailure::StreamFailure(const std::string& filePath)
    : std::ios_base::failure(filePath + " failure ") {
}

IncorrectSettingsFormat::IncorrectSettingsFormat(const std::string& settings)
    : std::invalid_argument(settings) {
}