#include "converters.hpp"

void conv::BassBoostConverter::process(std::vector<int16_t>& mainSample,
                                       std::vector<int16_t>& subSample) {

}

void conv::BassBoostConverter::setParams(conv::TaskParams& params) {
  taskInf_ = params;
}

size_t conv::BassBoostConverter::getReadSecond() {
  return taskInf_.curSec;
}


size_t conv::BassBoostConverter::getWriteSecond() {
  return taskInf_.curSec;
}

bool conv::BassBoostConverter::taskFinished() {
  return taskInf_.taskFinished;
}

size_t conv::BassBoostConverter::getReadStream() {
  return taskInf_.stream;
}
