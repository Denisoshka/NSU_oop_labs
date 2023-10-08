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
  SettingsFile_ = vm["config"].as<std::string>();
  FileOutPath_ = vm["output"].as<std::string>();
  FileLinks_ = vm["Input files"].as<std::vector<std::string>>();
}

void process::executeConversions() {
  conv::ConverterInterface interface;

  WAV::WAVReader wavReaderSub;
  WAV::WAVReader wavReaderOut{FileOutPath_};
  WAV::WAVWriter wavWriterOut{FileOutPath_};

  std::vector<int16_t> mainSampleOut;
  mainSampleOut.reserve(sampleRate_);
  std::vector<int16_t> mainSampleIn;
  mainSampleIn.reserve(sampleRate_);
  std::vector<int16_t> subSampleIn;
  subSampleIn.reserve(sampleRate_);

  wavWriterOut.writeHeader(WAV::stdRIFFChunk, WAV::stdFormatChunk, WAV::stdDataChunk);
  // todo fix algo
  size_t duration = 0;
  while( !interface.setTask() ) {
    wavReaderSub.open(interface.curFile());

    std::vector<std::vector<int16_t>> samples;
    while( !interface.taskFinished() || interface.curSec() < wavReaderSub.getDuration() ) {
      //      todo переделать эту @@@ на нормыльный выбор сэмплов
      if( interface.curFile() != FileOutPath_ ) {
        wavReaderSub.readSample(subSampleIn, interface.curSec());
        samples.push_back(subSampleIn);
      }
      else {
        wavReaderOut.readSample(mainSampleOut, interface.curSec());
        samples.push_back(mainSampleOut);
      }
      interface.executeTask(mainSampleOut, samples);
      wavWriterOut.writeSample(mainSampleOut, interface.curSec());
    }
    duration = (interface.curSec() > duration) ? interface.curSec() : duration;
  }

  WAV::RIFFChunk finalRiffChunk{WAV::stdRIFFChunk};
  finalRiffChunk.Size = (sizeof(WAV::stdRIFFChunk)
                         - (sizeof(WAV::stdRIFFChunk.Id) + sizeof(WAV::stdRIFFChunk.Size)))
                      + sizeof(WAV::stdFormatChunk) + sizeof(WAV::DataChunk)
                      + (duration * sizeof(uint16_t) * sampleRate_);
  WAV::DataChunk finalDataChunk{WAV::stdDataChunk};
  finalDataChunk.Size = duration * sizeof(uint16_t) * sampleRate_;
  wavWriterOut.writeHeader(finalRiffChunk, WAV::stdFormatChunk, finalDataChunk);
}
