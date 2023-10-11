#include "converter_exceptions.hpp"
#include "converters.hpp"

#include <boost/tokenizer.hpp>
#include <regex>

const int TaskTmpLen_ = 100;

bool conv::ConverterInterface::setTask() {
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

void conv::ConverterInterface::executeTask(std::vector<int16_t>& sampleOut,
                                           std::vector<int16_t>& samples) {
  curTask_->process(sampleOut, samples);
}

void conv::ConverterInterface::setSettings(const std::string& SettingsPath,
                                           const std::vector<std::string>& FileInLinks) {
  SettingsPath_ = SettingsPath;
  SettingsStream_.open(SettingsPath_, std::ios_base::in);
  if( SettingsStream_.fail() ) {
    throw StreamFailure(SettingsPath_);// todo
  }

  FileLinks_.push_back("_");
  for( const auto& link: FileInLinks ) {
    FileLinks_.push_back(link);
  }

  //  FileLinks_ = FileInLinks;
  //  FileLinks_.
  fillPipeline_();
}

void conv::ConverterInterface::fillPipeline_() {
  std::string task;
  boost::char_separator<char> sep(" ");

  while( Pipeline_.size() != TasksCount_ && !SettingsStream_.eof() ) {
    std::getline(SettingsStream_, task);
    if( SettingsStream_.fail() ) {
      throw StreamFailure(SettingsPath_);// todo
    }
    if( task.empty() || task[0] == '#' ) {
      continue;
    }
    TaskInf taskInf_;
    boost::tokenizer<boost::char_separator<char>> tokens(task, sep);
    size_t tokenPosition = 0;
    // todo переписать на boost po
    for( const std::string& token: tokens ) {
      if( tokenPosition == settingsPos::converter && regex_match(token, ConverterName_) ) {
        if( !converters_.contains(token) ) {
          throw UnckonwConverter(token);
        }
        taskInf_.converter = converters_[token];
      }
      else if( regex_match(token, Pass_) ) {
      }
      else if( tokenPosition == settingsPos::stream && (regex_match(token, StreamName_)) ) {
        taskInf_.params.stream = std::atoll(token.data() + 1);
      }
      else if( tokenPosition == settingsPos::timeStart && regex_match(token, Time_) ) {
        taskInf_.params.startTime = std::atoll(token.data());
      }
      else if( tokenPosition == settingsPos::timeEnd && regex_match(token, Time_) ) {
        taskInf_.params.endTime = std::atoll(token.data());
      }
      else if( tokenPosition > settingsPos::timeEnd && regex_match(token, Time_) ) {
        taskInf_.params.otherParams.push_back(std::atoll(token.data()));
      }
      else {
        throw IncorrectSettingsFormat(task);
      }
      tokenPosition++;
    }
    Pipeline_.push(std::move(taskInf_));
  }
}

/*

void conv::ConverterInterface::setFileLinks_(const std::vector<std::string>& fileLinks) {
  FileLinks_ = fileLinks;
}
*/

bool conv::ConverterInterface::taskFinished() {
  return curTask_->taskFinished();
}
// что то не так в наследовании todo
// конструктор вызывает эту функцию при инициализации

size_t conv::ConverterInterface::curStream() const {
  return curStream_;
}

std::string conv::ConverterInterface::curFile(const size_t stream) const {
  return FileLinks_[stream];
}

size_t conv::ConverterInterface::curReadSecond() const {
  return curTask_->getReadSecond();
}

size_t conv::ConverterInterface::curWriteSecond() const {
  return curTask_->getWriteSecond();
}

conv::ConverterInterface::ConverterInterface()
    : converters_({
            {"mix",  std::make_shared<MixConverter>() },
            {"mute", std::make_shared<MuteConverter>()},
}),
    ConverterName_(std::regex(R"(\w+)")), StreamName_(std::regex(R"(\$\d+)")),
    Time_(std::regex(R"(\d+)")), Pass_(std::regex(R"(--)")), TasksCount_(10), curStream_(0) {
}
