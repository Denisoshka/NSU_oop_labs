#include "converters.h"

#include <boost/tokenizer.hpp>
#include <regex>

/*std::string conv::ConverterInterface::getTask(size_t &FileIn, size_t &start, size_t &end) {
  if( taskList_.empty() ) {
    filTaskList();
  }

  std::string task = std::move(taskList_.back());
  taskList_.pop();
  return task;
}*/
bool conv::ConverterInterface::getTask() {
  SettingsStream_.open()
}

void conv::ConverterInterface::executeTask(sampleBuffer &sampleOut,
                                           std::vector<sampleBuffer &> &samples) {
  //  std::vector<size_t> params{secondsStart_, secondsEnd_, changedSeconds_, totalSeconds_};
  task_.converter.process(sampleOut, samples, task_.params);
  taskFinished_ = task_.params[0];
}

const int TaskTmpLen_ = 100;

void conv::ConverterInterface::filTaskList() {
  static const std::regex converterName(R"(\w+)");
  static const std::regex streamName(R"($[^-]\d+)");
  static const std::regex time(R"(\d+)");

  std::array<char, TaskTmpLen_> taskTmp{};

  while( taskList_.size() != TasksCount_ ) {
    if( SettingsStream_.getline(taskTmp.data(), TaskTmpLen_).fail() ) {
      throw;// (todo) make ex
    }

    if( taskTmp.front() == '#' ) {
      continue;
    }

    TaskInf_ taskInf_ ;
    boost::char_separator<char> sep(" ");
    boost::tokenizer<boost::char_separator<char>> tokens(taskTmp, sep);
    for( const auto &token: tokens ) {
      if( regex_match(token, converterName) ) {
        if( taskInf_.converter ) {
          throw;// (todo) make ex
        }
        taskInf_.converter = converterIndex[token];
      }
      else if( regex_match(token, converterName) ) {
        taskInf_.thread = std::atoll(token.data());
      }
      else if( regex_match(token, time) ) {
        size_t paramTime = std::atoll(token.data());
        taskInf_.params.push_back(paramTime);
      }
      else {
        throw;// (todo) make ex
      }
    }
    
  }
}

bool conv::ConverterInterface::taskFinished() const {
  return taskFinished_;
}

std::string conv::ConverterInterface::getCurFile() {
  return FileLinks_[fileInf_.CurFile_];
}

size_t conv::ConverterInterface::getCurSec() const {
  return fileInf_.CurSecond_;
}

void conv::ConverterInterface::open(std::string &&SettingsFile) {
  //  todo rvalue?
  SettingsPath_ = std::move(SettingsFile);
  SettingsStream_.open(SettingsPath_, std::ios_base::in);
  if( SettingsStream_.fail() ) {
    throw;
  }
}

/*conv::sampleBuffer::sampleBuffer(size_t sampleRate)
    : sample_(std::make_unique<uint16_t[]>(sampleRate))
    , curLen_(sampleRate)
    , len_(sampleRate) {
}*/
