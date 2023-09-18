#ifndef CONVERTER_INTERFACE_CONVERTER_H
#define CONVERTER_INTERFACE_CONVERTER_H


#include "converters.h"

#include <iostream>
#include <unordered_map>
#include <memory>


class converterInterface {
  private:
  std::unordered_map<std::string, std::unique_ptr<converter>> converters
          = {{"mute", std::make_unique<muteConverter>()},
             {"mix", std::make_unique<mixConverter>()}};


  public:
  virtual void process() = 0;
};

void getDescription();
#endif// CL_PARSER_CONVERTER_H
