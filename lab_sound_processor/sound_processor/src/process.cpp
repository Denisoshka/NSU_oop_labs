#include "process.hpp"
#include "converters.hpp"
#include "wav.hpp"

process::process(const po::variables_map& vm)
    : sampleRate_(WAV::SAMPLE_RATE)
    , SettingsFile_(vm["config"].as<std::string>())
    , FileOutPath_(vm["output"].as<std::string>())
    , FileLinks_(vm["input"].as<std::vector<std::string>>()) {
}

void process::setSettings(const po::variables_map& vm) {
  sampleRate_ = WAV::SAMPLE_RATE;
  SettingsFile_ = vm["config"].as<std::string>();
  FileOutPath_ = vm["output"].as<std::string>();
  FileLinks_ = vm["input"].as<std::vector<std::string>>();
}

void process::executeConversions() {
  WAV::WAVWriter wavWriterOut(FileOutPath_);
  wavWriterOut.writeHeader(WAV::stdRIFFChunk, WAV::stdFormatChunk, WAV::stdDataChunk);

  WAV::WAVReader wavReaderOut{FileOutPath_};
  WAV::WAVReader wavReaderSub;

  std::vector<int16_t> mainSampleIn;
  mainSampleIn.resize(sampleRate_);

  std::vector<int16_t> subSampleIn;
  subSampleIn.resize(sampleRate_);

  // todo fix algo
  conv::ConverterInterface interface;
  interface.setSettings(SettingsFile_, FileLinks_);
  while( interface.setTask() ) {
    if( interface.curStream() ) {
      wavReaderSub.open(interface.curFile(interface.curStream()));
    }

    size_t curDuration = (interface.curStream()) ? wavReaderSub.getDuration() : outDuration_;
    while( !interface.taskFinished() && interface.curReadSecond() < curDuration ) {
      //      size_t workSecond = interface.curSec();
      size_t readSecond = interface.curReadSecond();
      size_t writeSecond = interface.curWriteSecond();
      if( readSecond < outDuration_ ) {
        wavReaderOut.readSample(mainSampleIn, readSecond);
      }
      else {
        std::fill(mainSampleIn.begin(), mainSampleIn.end(), 0);
      }

      std::vector<int16_t>& mainSampleOut = (interface.curStream()) ? subSampleIn : mainSampleIn;
      wavReaderSub.readSample(mainSampleOut, readSecond);
      interface.executeTask(mainSampleOut, mainSampleIn);
      wavWriterOut.writeSample(mainSampleOut, writeSecond);

      outDuration_ = (interface.curWriteSecond() > outDuration_) ? interface.curWriteSecond()
                                                                 : outDuration_;
    }
  }


  WAV::DataChunk finalDataChunk{WAV::stdDataChunk};
  finalDataChunk.Size = outDuration_ * sizeof(uint16_t) * sampleRate_;

  WAV::RIFFChunk finalRiffChunk{WAV::stdRIFFChunk};
  finalRiffChunk.Size = WAV::FINAL_RIFF_CHUNK_SIZE_WITHOUT_DATA_SIZE
                      + outDuration_ * sizeof(uint16_t) * sampleRate_;


  wavWriterOut.writeHeader(finalRiffChunk, WAV::stdFormatChunk, finalDataChunk);
}
