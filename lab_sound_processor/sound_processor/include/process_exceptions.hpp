#pragma once

#include <ios>
#include <stdexcept>

class StreamFailure: public std::ios_base::failure {
public:
  explicit StreamFailure(const std::string& filePath);
};

class IncorrectSettingsFormat: public std::invalid_argument {
public:
  explicit IncorrectSettingsFormat(const std::string& settings);
};
