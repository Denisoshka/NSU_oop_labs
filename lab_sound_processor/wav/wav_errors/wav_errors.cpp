#include "wav_errors.h"

IncorrectFileFormat::IncorrectFileFormat(const std::string &filePath, const std::string &desc)
    : std::invalid_argument(filePath + ": Incorrect file format : " + desc) {
}

IncorrectExtension::IncorrectExtension(const std::string &filePath)
    : IncorrectFileFormat(filePath, "Incorrect file extension") {
}

FileNotOpen::FileNotOpen(const std::string &filePath)
    : std::ios_base::failure(filePath + " : File is not open ") {
}

FileNotWrite::FileNotWrite(const std::string &filePath)
    : std::ios_base::failure(filePath + " : can't to write in file ") {
}
