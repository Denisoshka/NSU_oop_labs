#include "process_exceptions.hpp"

StreamFailure::StreamFailure(const std::string& kFilePath)
    : std::ios_base::failure(kFilePath + " failure ") {
}

IncorrectSettingsFormat::IncorrectSettingsFormat(const std::string& kSettings)
    : std::invalid_argument(kSettings) {
}