#include "process.hpp"
#include "converters.hpp"
#include "process_exceptions.hpp"
#include "wav.hpp"

#include <boost/tokenizer.hpp>
#include <fstream>
#include <map>
#include <memory>
#include <regex>
#include <string>
#include <vector>

namespace {
  const size_t kDefSampleRate = 44100;
  const std::regex kDefConverterName_{std::regex(R"(\w+)")};
  const std::regex kDefStreamName_{std::regex(R"(\$\d+)")};
  const std::regex kDefTime{std::regex(R"(\d+)")};
  const std::regex kDefPass{std::regex(R"(--)")};
  const size_t kDefTasksCount = 10;
  const boost::char_separator<char> kDefSep(" ");

  enum ekSettingsPos {
    converter,
    stream,
  };
}// namespace

Process::Process(const boost::program_options::variables_map& kVM)
    : SampleRate_(kDefSampleRate)
    , FileInPath_(kVM["input"].as<std::vector<std::string>>())
    , SettingsPath_(kVM["config"].as<std::string>())
    , FileOutPath_(kVM["output"].as<std::string>())
    , SettingsStream_(std::ifstream(kVM["config"].as<std::string>())) {
}

void Process::setSettings(const boost::program_options::variables_map& kVM) {
  SampleRate_ = kDefSampleRate;

  SettingsPath_ = kVM["config"].as<std::string>();
  FileOutPath_ = kVM["output"].as<std::string>();
  FileInPath_ = kVM["input"].as<std::vector<std::string>>();
  SettingsStream_ = std::ifstream(kVM["config"].as<std::string>());
}

void Process::executeConversions() {
  WAV::makeWAVFile(FileOutPath_);
  WAV::WAVWriter wavWriterOut(FileOutPath_);
  wavWriterOut.writeHeader(OutDuration_);

  WAV::WAVReader wavReaderOut{FileOutPath_};
  WAV::WAVReader wavReaderSub;

  std::vector<int16_t> subSampleIn;
  subSampleIn.resize(SampleRate_);
  std::vector<int16_t> mainSampleOut{};
  mainSampleOut.resize(SampleRate_);

  Pipeline pipeline{SettingsPath_, kDefTasksCount};
  pipeline.fill();
  while( !pipeline.empty() ) {
    TaskInf task = pipeline.pop();
    if( pipeline.empty() ) {
      pipeline.fill();
    }

    std::unique_ptr<conv::Converter> converter = conv::makeConverter(task.converter, task.params);
    if( converter->getReadStream() ) {
      wavReaderSub.open(FileInPath_[converter->getReadStream()]);
    }

    size_t CurReadDuration =
            (converter->getReadStream()) ? wavReaderSub.getDuration() : OutDuration_;
    while( !converter->taskFinished() && converter->getReadSecond() < CurReadDuration ) {
      size_t readSecond = converter->getReadSecond();
      size_t writeSecond = converter->getWriteSecond();

      if( converter->getReadStream() && readSecond < CurReadDuration ) {
        wavReaderSub.readSample(mainSampleOut, readSecond);
      }
      else if( !converter->getReadStream() && writeSecond < CurReadDuration ) {
        wavReaderOut.readSample(mainSampleOut, writeSecond);
      }
      else {
        std::fill(mainSampleOut.begin(), mainSampleOut.end(), 0);
      }

      wavReaderSub.readSample(subSampleIn, readSecond);
      converter->process(mainSampleOut, subSampleIn);
      wavWriterOut.writeSample(mainSampleOut, writeSecond);

      OutDuration_ = (converter->getWriteSecond() > OutDuration_) ? converter->getWriteSecond()
                                                                  : OutDuration_;
    }
  }
  wavWriterOut.writeHeader(OutDuration_);
}

Pipeline::Pipeline(const std::string& kSettingsPath, const size_t kTasksCount)
    : tasksCount_(kTasksCount)
    , settingsPath_(kSettingsPath)
    , settingsStream_(std::ifstream(kSettingsPath, std::ios::in)) {
}

bool Pipeline::empty() const {
  return container_.empty();
}

TaskInf Pipeline::pop() {
  TaskInf Tmp = container_.front();
  container_.pop();
  return Tmp;
}

void Pipeline::fill() {
  while( container_.size() != tasksCount_ && !settingsStream_.eof() ) {
    std::string task;
    std::getline(settingsStream_, task);
    if( settingsStream_.fail() ) {
      throw StreamFailure(settingsPath_);// todo
    }
    if( task.empty() || task[0] == '#' ) {
      continue;
    }
    TaskInf taskInf_;
    boost::tokenizer<boost::char_separator<char>> tokens(task, kDefSep);
    size_t tokenPosition = 0;
    // todo переписать на boost po
    for( const std::string& token: tokens ) {
      if( tokenPosition == ekSettingsPos::converter && regex_match(token, kDefConverterName_) ) {
        taskInf_.converter = std::move(token);
      }
      else if( tokenPosition == ekSettingsPos::stream && (regex_match(token, kDefStreamName_)) ) {
        taskInf_.params.push_back(std::atoll(token.data() + 1));
      }
      else if( tokenPosition > ekSettingsPos::stream && regex_match(token, kDefTime) ) {
        taskInf_.params.push_back(std::atoll(token.data()));
      }
      else if( regex_match(token, kDefPass) ) {
        taskInf_.params.push_back(SIZE_MAX);
      }
      else {
        throw IncorrectSettingsFormat(task);
      }

      tokenPosition++;
    }
    container_.push(taskInf_);
  }
}
