#ifndef WAV_WAV_ERRORS_H
#define WAV_WAV_ERRORS_H

#include <iostream>

class IncorrectFileFormat : public std::invalid_argument
{
protected:
  explicit IncorrectFileFormat(const std::string & file_path,
                               const std::string & desc);
};


#endif// WAV_WAV_ERRORS_H
