#include "converter.hpp"

size_t conv::Converter::getReadSecond() {
  return TaskInf_.CurTime;
}

size_t conv::Converter::getWriteSecond() {
  return TaskInf_.CurTime;
}

bool conv::Converter::taskFinished() {
  return TaskInf_.TaskFinished;
}

size_t conv::Converter::getReadStream() {
  return TaskInf_.Stream;
}

void conv::Converter::setParams(const std::vector<size_t>& kParams) {
  TaskInf_.Stream = (kParams[0] == SIZE_MAX) ? 0 : kParams[0];
  TaskInf_.StartTime = (kParams[1] == SIZE_MAX) ? 0 : kParams[1];
  TaskInf_.CurTime = TaskInf_.StartTime;
  TaskInf_.EndTime = (kParams[2] == SIZE_MAX) ? SIZE_MAX - 1 : kParams[2];
  TaskInf_.TaskFinished = false;
  for( size_t i = 3; i < kParams.size(); ++i ) {
    TaskInf_.OtherParams.push_back(kParams[i]);
  }
}

conv::Converter::Converter(const std::vector<size_t>& kParams) {
  Converter::setParams(kParams);
}
