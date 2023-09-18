//
// Created by denis on 17.09.23.
//

#ifndef WAV_WAV_ERRORS_H
#define WAV_WAV_ERRORS_H

#include <ios>
#include <iostream>

class FileNotOpen: std::ios_base::failure {
public:
  explicit FileNotOpen(const std::string &filePath);
};

class FileNotWrite: public std::ios_base::failure {
public:
  explicit FileNotWrite(const std::string &filePath);
};

class IncorrectFileFormat: public std::invalid_argument {
protected:
  explicit IncorrectFileFormat(const std::string &filePath, const std::string &desc);
};

class IncorrectExtension: public IncorrectFileFormat {
public:
  explicit IncorrectExtension(const std::string &filePath);
};


#endif// WAV_WAV_ERRORS_H
