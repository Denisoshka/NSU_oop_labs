#include "converter.hpp"

size_t conv::Converter::getReadSecond() {
  return taskInf_.curTime;
}

size_t conv::Converter::getWriteSecond() {
  return taskInf_.curTime;
}

bool conv::Converter::taskFinished() {
  return taskInf_.taskFinished;
}

size_t conv::Converter::getReadStream() {
  return taskInf_.stream;
}

void conv::Converter::setParams(const std::vector<size_t>& kParams) {
  taskInf_.stream = (kParams[0] == SIZE_MAX) ? 0 : kParams[0];
  taskInf_.startTime = (kParams[1] == SIZE_MAX) ? 0 : kParams[1];
  taskInf_.curTime = taskInf_.startTime;
  taskInf_.endTime = (kParams[2] == SIZE_MAX) ? SIZE_MAX - 1 : kParams[2];
  taskInf_.taskFinished = false;
  for( size_t i = 3; i < kParams.size(); ++i ) {
    taskInf_.otherParams.push_back(kParams[i]);
  }
}

conv::Converter::Converter(const std::vector<size_t>& kParams) {
  Converter::setParams(std::move(kParams));
}
