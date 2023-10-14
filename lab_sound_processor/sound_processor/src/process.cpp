#include "process.hpp"
#include <boost/tokenizer.hpp>
#include "converters.hpp"
#include "wav.hpp"

#include <fstream>
#include <map>
#include <memory>
#include <queue>
#include <regex>
#include <string>
#include <vector>

process::process(boost::program_options::variables_map& vm)
    : SampleRate_(WAV::SAMPLE_RATE)
    , FileInPath_(vm["input"].as<std::vector<std::string>>())
    , SettingsPath_(vm["config"].as<std::string>())
    , FileOutPath_(vm["output"].as<std::string>()) {
}

void process::setSettings(boost::program_options::variables_map& vm) {
  SampleRate_ = WAV::SAMPLE_RATE;
  SettingsPath_ = vm["config"].as<std::string>();
  FileOutPath_ = vm["output"].as<std::string>();
  FileInPath_ = vm["input"].as<std::vector<std::string>>();
}

void process::executeConversions() {
  WAV::makeWAVFile(FileOutPath_);
  WAV::WAVWriter wavWriterOut(FileOutPath_);
  wavWriterOut.writeHeader(WAV::stdRIFFChunk, WAV::stdFormatChunk, WAV::stdDataChunk);

  WAV::WAVReader wavReaderOut{FileOutPath_};
  WAV::WAVReader wavReaderSub;

  std::vector<int16_t> subSampleIn;
  subSampleIn.resize(SampleRate_);
  std::vector<int16_t> mainSampleOut{};
  mainSampleOut.resize(SampleRate_);


  while( !Pipeline_.empty() ) {
    TaskInf task = Pipeline_.front();
    Pipeline_.pop();
    conv::TaskParams params = conv::convertToCONVParams(std::move(task.params));
    std::unique_ptr<conv::Converter> converter =
            conv::makeConverter(task.converter, std::move(params));

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

  WAV::DataChunk finalDataChunk{WAV::stdDataChunk};
  finalDataChunk.Size = OutDuration_ * sizeof(uint16_t) * SampleRate_;
  WAV::RIFFChunk finalRiffChunk{WAV::stdRIFFChunk};
  finalRiffChunk.Size = WAV::FINAL_RIFF_CHUNK_SIZE_WITHOUT_DATA_SIZE
                      + OutDuration_ * sizeof(uint16_t) * SampleRate_;

  wavWriterOut.writeHeader(finalRiffChunk, WAV::stdFormatChunk, finalDataChunk);
}

const std::regex ConverterName_ = std::regex(R"(\w+)");
const std::regex StreamName_ = std::regex(R"(\$\d+)");
const std::regex Time_ = std::regex(R"(\d+)");
const std::regex Pass_ = std::regex(R"(--)");
const size_t TasksCount_ = 10;

void fillPipeline_() {

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
        taskInf_.converter = ;
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
    Pipeline_.push(taskInf_);
  }
}
