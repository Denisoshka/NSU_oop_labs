#include "converters.h"

std::string ConverterInterface::getTask(size_t &FileIn,size_t &start, size_t &end ) {
  if( taskList_.empty() ) {
    filTaskList();
  }

  std::string task = std::move(taskList_.back());
  taskList_.pop();
  return task;
}

void ConverterInterface::executeTask(uint16_t *sampleBuffer, size_t sampleBufferLen) {
  Converter converter = getCongerter(task_);

  for ( ;;){

  }
}


void ConverterInterface::filTaskList() {
}
