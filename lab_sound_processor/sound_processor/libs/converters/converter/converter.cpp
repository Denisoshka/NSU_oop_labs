#include "converter.hpp"

conv::TaskParams convertToCONVparams(std::vector<size_t>&& params) {
  conv::TaskParams task{.curTime = 0};
  task.stream = params[0];
  //  params.erase(params.begin());
  task.startTime = params[1];
  //  params.erase(params.begin());
  task.endTime = params[3];
  //  params.erase(params.begin());
  task.taskFinished = static_cast<bool>(params[4]),
  //  params.erase(params.begin());
//          params.erase(params.begin(), params.begin() + 4);
  std::move(params.begin() + 5, params.end(), task.otherParams.begin());
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

void conv::Converter::setParams(conv::TaskParams&& params) {
  taskInf_ = std::move(params);
  taskInf_.curTime = taskInf_.startTime;
}

conv::Converter::Converter(conv::TaskParams&& params) {
  taskInf_ = std::move(params);
  taskInf_.curTime = taskInf_.startTime;
}
