#include "Converter.h"

#include <boost/program_options.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>

void getDescription() {
  std::string json_filename = "example.json";// Имя файла с JSON-данными

  // Создайте пустое дерево свойств
  boost::property_tree::ptree pt;

  // Десериализуйте JSON из файла в дерево свойств
  try {
    boost::property_tree::read_json(json_filename, pt);
  } catch (const boost::property_tree::json_parser_error& e) {
    std::cerr << "Ошибка при чтении JSON из файла: " << e.what() << std::endl;
  }
}

void getConverter(const std::string& converterName) {
  std::unordered_map<std::string, std::unique_ptr<converter>> converters = {
          {"mute", std::make_unique<muteConverter>()},
          {"mix", std::make_unique<mixConverter>()}
  };
}
