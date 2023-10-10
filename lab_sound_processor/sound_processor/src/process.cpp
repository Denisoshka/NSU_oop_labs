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

  std::vector<int16_t> mainSampleIn;
  mainSampleIn.resize(sampleRate_);

  std::vector<int16_t> subSampleIn;
  subSampleIn.resize(sampleRate_);

  // todo fix algo
  interface.setSettings(SettingsFile_, FileLinks_);
  while( interface.setTask() ) {
    if( interface.curStream() ) {
      wavReaderSub.open(interface.curFile(interface.curStream()));
    }

    size_t curDuration = (interface.curStream()) ? wavReaderSub.getDuration() : outDuration_;
    while( !interface.taskFinished() && interface.curSec() < curDuration ) {
      size_t workSecond = interface.curSec();
      if( workSecond < outDuration_ ) {
        wavReaderOut.readSample(mainSampleIn, workSecond);
      }
      else {
        std::fill(mainSampleIn.begin(), mainSampleIn.end(), 0);
      }

      std::vector<int16_t>& mainSampleOut = (interface.curStream()) ? subSampleIn : mainSampleIn;
      wavReaderSub.readSample(mainSampleOut, workSecond);
      interface.executeTask(mainSampleOut, mainSampleIn);
      wavWriterOut.writeSample(mainSampleOut, workSecond);

      outDuration_ = (interface.curSec() > outDuration_) ? interface.curSec() : outDuration_;
    }
  }

  WAV::RIFFChunk finalRiffChunk{WAV::stdRIFFChunk};
  finalRiffChunk.Size = (sizeof(WAV::stdRIFFChunk)
                         - (sizeof(WAV::stdRIFFChunk.Id) + sizeof(WAV::stdRIFFChunk.Size)))
                      + sizeof(WAV::stdFormatChunk) + sizeof(WAV::DataChunk)
                      + (outDuration_ * sizeof(uint16_t) * sampleRate_);
  WAV::DataChunk finalDataChunk{WAV::stdDataChunk};
  finalDataChunk.Size = outDuration_ * sizeof(uint16_t) * sampleRate_;
  wavWriterOut.writeHeader(finalRiffChunk, WAV::stdFormatChunk, finalDataChunk);
}
