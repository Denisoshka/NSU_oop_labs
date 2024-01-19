#include "process.hpp"
#include "converters.hpp"
#include "process_exceptions.hpp"
#include "wav.hpp"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/tokenizer.hpp>
#include <fstream>
#include <iostream>
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
  const std::string missingField{"missing field"};

  enum ekSettingsPos {
    ekConverter,
  };
}// namespace

process::Process::Process(const boost::program_options::variables_map& kVM)
    : SampleRate_(kDefSampleRate)
    , FileInPath_(kVM["input"].as<std::vector<std::string>>())
    , SettingsPath_(kVM["config"].as<std::string>())
    , FileOutPath_(kVM["output"].as<std::string>()) {
}

void process::Process::setSettings(const boost::program_options::variables_map& kVM) {
  SampleRate_ = kDefSampleRate;
  SettingsPath_ = kVM["config"].as<std::string>();
  FileOutPath_ = kVM["output"].as<std::string>();
  FileInPath_ = kVM["input"].as<std::vector<std::string>>();
}

void process::Process::executeConversions() {
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
    std::unique_ptr<conv::Converter> converter =
            conv::makeConverter(task.Converter, task.InStreams, task.Params);

    const size_t streamQuantity = converter->inStreamQuantity();
    for( size_t streamNum = 0; streamNum < streamQuantity; ++streamNum ) {
      if( converter->getReadStream() ) {
        if (converter->getReadStream() > FileInPath_.size()){
          throw IncorrectStreamNumber(converter->getReadStream());
        }
        wavReaderSub.open(FileInPath_[converter->getReadStream()-1]);
      }
      size_t curReadDuration =
              (converter->getReadStream()) ? wavReaderSub.getDuration() : OutDuration_;
      converter->setInDuration(curReadDuration);

      while( !converter->taskFinished() ) {
        size_t readSecond = converter->getReadSecond();
        size_t writeSecond = converter->getWriteSecond();

        if( writeSecond < OutDuration_ ) {
          wavReaderOut.readSample(mainSampleOut, writeSecond);
        }
        else {
          std::fill(mainSampleOut.begin(), mainSampleOut.end(), 0);
        }

        if( converter->getReadStream()) {
          wavReaderSub.readSample(subSampleIn, readSecond);
        }

        converter->process(mainSampleOut, subSampleIn);
        wavWriterOut.writeSample(mainSampleOut, writeSecond);

        OutDuration_ = std::max(converter->getWriteSecond(), OutDuration_);
      }
    }
  }
  wavWriterOut.writeHeader(OutDuration_);
}

process::Pipeline::Pipeline(const std::string& kSettingsPath, const size_t kTasksCount)
    : TasksCount_(kTasksCount)
    , SettingsPath_(kSettingsPath)
    , SettingsStream_(std::ifstream(kSettingsPath, std::ios::in)) {
}

bool process::Pipeline::empty() const {
  return Container_.empty();
}

process::TaskInf process::Pipeline::pop() {
  TaskInf Tmp = Container_.front();
  Container_.pop();
  return Tmp;
}

void process::Pipeline::fill() {
  while( Container_.size() != TasksCount_ && !SettingsStream_.eof() ) {
    std::string task;
    std::getline(SettingsStream_, task);
    if( SettingsStream_.fail() ) {
      throw process::StreamFailure(SettingsPath_);// todo
    }
    if( task.empty() || task[0] == '#' ) {
      continue;
    }
    TaskInf taskInf_;
    boost::tokenizer<boost::char_separator<char>> tokens(task, kDefSep);
    size_t tokenPosition = 0;
    for( const std::string& token: tokens ) {
      if( tokenPosition == ekSettingsPos::ekConverter && regex_match(token, kDefConverterName_) ) {
        taskInf_.Converter = std::move(token);
      }
      else if( tokenPosition > ekSettingsPos::ekConverter
               && (regex_match(token, kDefStreamName_)) ) {
        taskInf_.InStreams.push_back(std::atoll(token.data() + 1));
      }
      else if( tokenPosition > ekSettingsPos::ekConverter && regex_match(token, kDefTime) ) {
        taskInf_.Params.push_back(std::atoll(token.data()));
      }
      else if( regex_match(token, kDefPass) ) {
        (taskInf_.InStreams.empty()) ? taskInf_.InStreams.push_back(SIZE_MAX)
                                     : taskInf_.Params.push_back(SIZE_MAX);
      }
      else {
        throw process::IncorrectSettingsFormat(task);
      }

      tokenPosition++;
    }
    Container_.push(taskInf_);
  }
}

void process::printConverterDesc(const std::string& kProgramName, const std::string& kUsage,
                                 const std::string& kDescription) {
  boost::property_tree::ptree jsonConvertersTree;
  std::stringstream ss{kDescription};
  boost::property_tree::read_json(ss, jsonConvertersTree);
  const boost::property_tree::ptree& kConvertersDeskArray =
          jsonConvertersTree.get_child("converters");
  const int indent = 30;
  std::cout << kProgramName << " " << kUsage << std::endl;
  std::cout << std::left << std::setw(indent) << "Сonverter"
            << " Description" << std::endl;

  for( const auto& kConverterDesk: kConvertersDeskArray ) {

    std::cout << std::left << std::setw(indent)
              << kConverterDesk.second.get<std::string>("name", missingField) << std::right
              << kConverterDesk.second.get<std::string>("params", missingField) << "\n"
              << std::setw(indent) << std::left << " "
              << kConverterDesk.second.get<std::string>("description", missingField) << "\n"
              << std::endl;
  }
}
