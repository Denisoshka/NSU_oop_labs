#include "converters.hpp"

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
  return InStream_;
}

void conv::Converter::setParams(const std::vector<size_t>& kInStreams,
                                const std::vector<size_t>& kParams) {
  InStream_ = (kInStreams[0] == SIZE_MAX) ? 0 : kInStreams[0];
  TaskInf_.StartTime = (kParams[0] == SIZE_MAX) ? 0 : kParams[0];
  TaskInf_.CurTime = TaskInf_.StartTime;
  TaskInf_.EndTime = (kParams[1] == SIZE_MAX) ? SIZE_MAX - 1 : kParams[1];
  TaskInf_.TaskFinished = false;
}

conv::Converter::Converter(const std::vector<size_t>& kInStreams,
                           const std::vector<size_t>& kParams) {
  Converter::setParams(kInStreams, kParams);
}

size_t conv::Converter::inStreamQuantity() const {
  return 1;
}

void conv::Converter::setInDuration(const size_t InDuration) {
  TaskInf_.InDuration = InDuration;
}
