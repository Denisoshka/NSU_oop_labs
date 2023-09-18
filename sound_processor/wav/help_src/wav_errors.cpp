#include "../help_include/wav_types.h"

#include <string>

IncorrectFileFormat::IncorrectFileFormat(const std::string & file_path,
                                         const std::string & desc)
    : std::invalid_argument(file_path + ": Incorrect file format : " + desc) {}
