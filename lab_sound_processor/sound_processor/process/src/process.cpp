#include <process.h>

#include "converters/include/converters.h"
#include "wav/include/wav.h"

// #include "converters/converter_tmp/converter_tmp.h"
// namespace pt = boost::property_tree;

conv::sampleBuffer convertFromWAVSampleBuffer(WAV::SampleBuffer &tmp) {
  return {tmp.get(), tmp.size(), tmp.size()};
}

process::process(const size_t sampleRate)
    : sampleRate_(sampleRate) {
}

process::process(const size_t sampleRate, po::variables_map &vm)
    : sampleRate_(sampleRate)
    , SettingsFile_(vm["config"].as<std::string>())
    , FileOutPath_(vm["output"].as<std::string>())
    , FileLinks_(vm["input"].as<std::vector<std::string>>()) {
}

void process::setSettings(po::variables_map &vm) {
  SettingsFile_ = vm["config"].as<std::string>();
  FileOutPath_ = vm["output"].as<std::string>();
  FileLinks_ = vm["Input files"].as<std::vector<std::string>>();
}

void process::executeConversions() {
  //  FilePath_ = FilePath;
  /*
    FileOut_.open(FilePath_, std::ios_base::binary);
    if( FileOut_.fail() ) {
      throw;// todo make ex
    }
  */

  conv::ConverterInterface interface;
  conv::sampleBuffer mainBufOut{};
  conv::sampleBuffer subBufIn{};

  WAV::WAVReader wavReaderSub;
  WAV::WAVReader wavReaderOut{FileOutPath_};
  WAV::WAVWriter wavWriterOut{FileOutPath_};
  WAV::SampleBuffer mainSampleOut{sampleRate_};
  WAV::SampleBuffer mainSampleIn{sampleRate_};
  WAV::SampleBuffer subSampleIn{sampleRate_};

  // todo fix algo
  while( true ) {
    if( !interface.setTask() ) {
      break;
    }
//    wavReaderSub.open(interface.curFile());

    std::vector<conv::sampleBuffer> samples;
    while( !interface.taskFinished() || interface.curSec() < wavReaderSub.getDuration() ) {

//      todo переделать эту поеботу на нормыльный выбор сэмплов
      if (interface.curFile() != FileOutPath_){
        wavReaderSub.getSample(subSampleIn, interface.curSec());
        samples[0] = convertFromWAVSampleBuffer(subSampleIn);
      }
      else{
        wavReaderOut.getSample(mainSampleOut, interface.curSec());
        samples[0] = convertFromWAVSampleBuffer(mainSampleOut);
      }

      mainBufOut = convertFromWAVSampleBuffer(mainSampleOut);
      interface.executeTask(mainBufOut, samples);
      wavWriterOut.writeSample(mainSampleOut, interface.curSec());
    }
  }
  wavWriterOut.writeHeader();
}
