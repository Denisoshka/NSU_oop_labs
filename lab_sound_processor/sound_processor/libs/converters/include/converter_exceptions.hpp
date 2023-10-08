#ifndef CONVERTERS_CONVERTER_EXCEPTIONS_HPP
#define CONVERTERS_CONVERTER_EXCEPTIONS_HPP

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

class UnckonwConverter: public std::invalid_argument {
public:
  explicit UnckonwConverter(const std::string& converter);
};

#endif// CONVERTERS_CONVERTER_EXCEPTIONS_HPP
