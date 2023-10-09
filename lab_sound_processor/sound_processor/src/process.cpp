#include "process.hpp"
#include "converters.hpp"
#include "wav.hpp"

// #include "converters/converter_tmp/converter_tmp.h"
// namespace pt = boost::property_tree;

process::process(const po::variables_map& vm)
    : sampleRate_(WAV::SAMPLE_RATE)
    , SettingsFile_(vm["config"].as<std::string>())
    , FileOutPath_(vm["output"].as<std::string>())
    , FileLinks_(vm["input"].as<std::vector<std::string>>()) {
}

void process::setSettings(po::variables_map& vm) {
  sampleRate_ = WAV::SAMPLE_RATE;
  SettingsFile_ = vm["config"].as<std::string>();
  FileOutPath_ = vm["output"].as<std::string>();
  FileLinks_ = vm["input"].as<std::vector<std::string>>();
}

void process::executeConversions() {
  conv::ConverterInterface interface;

  WAV::WAVWriter wavWriterOut(FileOutPath_);
  wavWriterOut.writeHeader(WAV::stdRIFFChunk, WAV::stdFormatChunk, WAV::stdDataChunk);

  WAV::WAVReader wavReaderOut{FileOutPath_};
  WAV::WAVReader wavReaderSub;

  std::vector<int16_t> mainSampleOut;
  mainSampleOut.resize(sampleRate_);
  std::vector<int16_t> mainSampleIn;
  mainSampleIn.resize(sampleRate_);
  std::vector<int16_t> subSampleIn;
  subSampleIn.resize(sampleRate_);
  // todo fix algo
  interface.setSettings(SettingsFile_, FileLinks_);
  while( interface.setTask() ) {
    size_t curStreamIn = interface.curStream();

    if( curStreamIn ) {
      wavReaderSub.open(interface.curFile(curStreamIn));
    }

    size_t curDuration = (curStreamIn) ? wavReaderSub.getDuration() : duration_;
    while( !interface.taskFinished() && interface.curSec() < curDuration ) {
      //      todo переделать эту @@@ на нормыльный выбор сэмплов
      size_t workSecond = interface.curSec();
      // отдебажить алгос
      if( workSecond < duration_ ) {
        wavReaderOut.readSample(mainSampleOut, workSecond);
      }
      else {
        std::fill(mainSampleOut.begin(), mainSampleOut.end(), 0);
      }

      if( curStreamIn ) {
        wavReaderSub.readSample(subSampleIn, workSecond);
        interface.executeTask(mainSampleOut, subSampleIn);
      }
      else {
        interface.executeTask(mainSampleOut, mainSampleOut);
      }
      //    при mute мы читаем не наши секунды

      wavWriterOut.writeSample(mainSampleOut, workSecond);

      duration_ = (interface.curSec() > duration_) ? interface.curSec() : duration_;
    }
  }

  WAV::RIFFChunk finalRiffChunk{WAV::stdRIFFChunk};
  finalRiffChunk.Size = (sizeof(WAV::stdRIFFChunk)
                         - (sizeof(WAV::stdRIFFChunk.Id) + sizeof(WAV::stdRIFFChunk.Size)))
                      + sizeof(WAV::stdFormatChunk) + sizeof(WAV::DataChunk)
                      + (duration_ * sizeof(uint16_t) * sampleRate_);
  WAV::DataChunk finalDataChunk{WAV::stdDataChunk};
  finalDataChunk.Size = duration_ * sizeof(uint16_t) * sampleRate_;
  wavWriterOut.writeHeader(finalRiffChunk, WAV::stdFormatChunk, finalDataChunk);
}
