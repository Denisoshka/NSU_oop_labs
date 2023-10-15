#include "converter.hpp"

conv::TaskParams conv::convertToCONVParams(std::vector<size_t>&& params) {
  conv::TaskParams task{.stream = (params[0] == SIZE_MAX) ? 0 : params[0],
                        .startTime = (params[1] == SIZE_MAX) ? 0 : params[1],
                        .curTime = 0,
                        .endTime = (params[2] == SIZE_MAX) ? SIZE_MAX - 1 : params[2],
                        .taskFinished = false};
  for( size_t i = 3; i < params.size(); ++i ) {
    task.otherParams.push_back(params[i]);
  }
  return task;
}

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

void conv::Converter::setParams(std::vector<size_t>&& params) {
  taskInf_ = conv::convertToCONVParams(std::move(params));
  taskInf_.curTime = taskInf_.startTime;
}

conv::Converter::Converter(std::vector<size_t>&& params) {
  Converter::setParams(std::move(params));
}
