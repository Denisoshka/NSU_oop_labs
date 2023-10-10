#include "converters.hpp"


void conv::Converter::setParams(conv::TaskParams&& params) {
  taskInf_ = std::move(params);
}

bool conv::Converter::taskFinished() {
  return taskInf_.taskFinished;
}

size_t conv::Converter::getReadSecond() {
  return taskInf_.curSec;
}

size_t conv::Converter::getWriteSecond() {
  return taskInf_.curSec;
}
