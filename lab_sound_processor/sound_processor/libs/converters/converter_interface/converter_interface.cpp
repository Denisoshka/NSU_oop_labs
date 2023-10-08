#include "converter_exceptions.hpp"
#include "converters.hpp"

#include <boost/tokenizer.hpp>
#include <regex>
const int TaskTmpLen_ = 100;

bool conv::ConverterInterface::setTask() {
  if( Pipeline_.empty() ) {
    fillPipeline();
  }
  if (Pipeline_.empty()){
    return false;
  }

  curTask_ = std::move(Pipeline_.front());
  Pipeline_.pop();
  TaskFinished_ = false;

  return true;
}

void conv::ConverterInterface::executeTask(sampleBuffer &sampleOut,
                                           std::vector<sampleBuffer> &samples) {
  //  std::vector<size_t> params{secondsStart_, secondsEnd_, changedSeconds_};
  curTask_.converter->process(sampleOut, samples, curTask_.params);
  TaskFinished_ = curTask_.params[0];
}

void conv::ConverterInterface::setSettings(std::string &FilePath,
                                           std::vector<std::string> &fileLinks) {
  SettingsPath_ = FilePath;
  SettingsStream_.open(SettingsPath_, std::ios_base::in);
  if( SettingsStream_.fail() ) {
    throw StreamFailure(SettingsPath_);// todo make ex
  }

  setFileLinks(fileLinks);
  fillPipeline();
}

void conv::ConverterInterface::fillPipeline() {
  std::array<char, TaskTmpLen_> taskTmp{};
  //  std::string taskTmp;
  boost::char_separator<char> sep(" ");

  while( Pipeline_.size() != TasksCount_ || !SettingsStream_.eof() ) {
    if( SettingsStream_.getline(taskTmp.data(), TaskTmpLen_).fail() ) {
      throw StreamFailure(SettingsPath_);// (todo) make ex
    }
    if( taskTmp.front() == '#' ) {
      continue;
    }
    TaskInf_ taskInf_;
    boost::tokenizer<boost::char_separator<char>> tokens(taskTmp, sep);
    size_t tokenPosition = 0;
    // todo переписать на boost po
    for( const std::string &token: tokens ) {
      if( tokenPosition == settingsPos::converter && regex_match(token, ConverterName_) ) {
        if( !converters_.contains(token) ) {
          throw UnckonwConverter(token);
        }
        taskInf_.converter = converters_[token];
      }
      else if( regex_match(token, Pass_) ) {
      }
      else if( tokenPosition == settingsPos::stream && (regex_match(token, StreamName_)) ) {
        taskInf_.stream = std::atoll(token.data());
      }
      else if( tokenPosition == settingsPos::timeStart && regex_match(token, Time_) ) {
        taskInf_.startTime = std::atoll(token.data());
      }
      else if( tokenPosition == settingsPos::timeEnd ) {
        taskInf_.endTime = std::atoll(token.data());
      }
      else {
        throw IncorrectSettingsFormat(token);
      }
      tokenPosition++;
    }
    Pipeline_.push(std::move(taskInf_));
  }
}

void conv::ConverterInterface::setFileLinks(std::vector<std::string> &fileLinks) {
  FileLinks_ = fileLinks;
}

void conv::ConverterInterface::setFileLinks(const std::vector<std::string> &fileLinks) {
  FileLinks_ = fileLinks;
}

bool conv::ConverterInterface::taskFinished() const {
  return TaskFinished_;
}

std::string conv::ConverterInterface::curFile() {
  return FileLinks_[curTask_.stream];
}

size_t conv::ConverterInterface::curSec() const {
  // params{taskFinished(0), secondsStart(1), secondsEnd(2), changedSeconds(3), totalSeconds(4)}
  return curTask_.params[3];
}

/*
void conv::ConverterInterface::open(std::string &&SettingsFile) {
  //  todo rvalue?
  SettingsPath_ = std::move(SettingsFile);
  SettingsStream_.open(SettingsPath_, std::ios_base::in);
  if( SettingsStream_.fail() ) {
    throw;
  }
}
*/

/*conv::sampleBuffer::sampleBuffer(size_t sampleRate)
    : sample_(std::make_unique<uint16_t[]>(sampleRate))
    , curLen_(sampleRate)
    , len_(sampleRate) {
}*/
