#include "pipeline.hpp"

#include "converter_exceptions.hpp"
#include "converters.hpp"

const int TaskTmpLen_ = 100;

bool conv::ConverterPipeline::setTask() {
  if( Pipeline_.empty() ) {
    fillPipeline_();
  }
  if( Pipeline_.empty() ) {
    return false;
  }
  //  TaskInf taskInf = std::move();
  curStream_ = Pipeline_.front().params.stream;
  curTask_ = Pipeline_.front().converter;
  curTask_->setParams(Pipeline_.front().params);
  Pipeline_.pop();

  return true;
}

void conv::ConverterPipeline::executeTask(std::vector<int16_t>& sampleOut,
                                          std::vector<int16_t>& samples) {
  curTask_->process(sampleOut, samples);
}

void conv::ConverterPipeline::setSettings(std::string&& SettingsPath,
                                          std::vector<std::string>&& FileInLinks) {
  SettingsPath_ = std::move(SettingsPath);
  SettingsStream_.open(SettingsPath_, std::ios_base::in);
  if( SettingsStream_.fail() ) {
    throw StreamFailure(SettingsPath_);// todo
  }

  FileLinks_.push_back("_");
  for( auto&& link: FileInLinks ) {
    FileLinks_.push_back(std::move(link));
  }

  fillPipeline_();
}

bool conv::ConverterPipeline::taskFinished() {
  return curTask_->taskFinished();
}

std::string conv::ConverterPipeline::curFile(const size_t stream) const {
  return FileLinks_[stream];
}

size_t conv::ConverterPipeline::curReadSecond() const {
  return curTask_->getReadSecond();
}

size_t conv::ConverterPipeline::curWriteSecond() const {
  return curTask_->getWriteSecond();
}

size_t conv::ConverterPipeline::curReadStream() const {
  return curTask_->getReadStream();
}

conv::ConverterPipeline::ConverterPipeline()
    : TasksCount_(10)
    , curStream_(0) {
}
