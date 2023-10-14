#include "process.hpp"
#include "converters.hpp"
#include "wav.hpp"

process::process(po::variables_map& vm)
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
  WAV::makeWAVFile(FileOutPath_);
  WAV::WAVWriter wavWriterOut(FileOutPath_);
  wavWriterOut.writeHeader(WAV::stdRIFFChunk, WAV::stdFormatChunk, WAV::stdDataChunk);

  WAV::WAVReader wavReaderOut{FileOutPath_};
  WAV::WAVReader wavReaderSub;

  std::vector<int16_t> subSampleIn;
  subSampleIn.resize(sampleRate_);
  std::vector<int16_t> mainSampleOut{};
  mainSampleOut.resize(sampleRate_);

  conv::ConverterPipeline pipeline{};
  pipeline.setSettings(std::move(SettingsFile_), std::move(FileLinks_));
  while( pipeline.setTask() ) {
    if( pipeline.curReadStream() ) {
      wavReaderSub.open(pipeline.curFile(pipeline.curReadStream()));
    }

    size_t CurReadDuration = (pipeline.curReadStream()) ? wavReaderSub.getDuration() : outDuration_;
    while( !pipeline.taskFinished() && pipeline.curReadSecond() < CurReadDuration ) {
      size_t readSecond = pipeline.curReadSecond();
      size_t writeSecond = pipeline.curWriteSecond();

      if( pipeline.curReadStream() && readSecond < CurReadDuration ) {
        wavReaderSub.readSample(mainSampleOut, readSecond);
      }
      else if( !pipeline.curReadStream() && writeSecond < CurReadDuration ) {
        wavReaderOut.readSample(mainSampleOut, writeSecond);
      }
      else {
        std::fill(mainSampleOut.begin(), mainSampleOut.end(), 0);
      }

      wavReaderSub.readSample(subSampleIn, readSecond);
      pipeline.executeTask(mainSampleOut, subSampleIn);
      wavWriterOut.writeSample(mainSampleOut, writeSecond);

      outDuration_ =
              (pipeline.curWriteSecond() > outDuration_) ? pipeline.curWriteSecond() : outDuration_;
    }
  }

  WAV::DataChunk finalDataChunk{WAV::stdDataChunk};
  finalDataChunk.Size = outDuration_ * sizeof(uint16_t) * sampleRate_;
  WAV::RIFFChunk finalRiffChunk{WAV::stdRIFFChunk};
  finalRiffChunk.Size = WAV::FINAL_RIFF_CHUNK_SIZE_WITHOUT_DATA_SIZE
                      + outDuration_ * sizeof(uint16_t) * sampleRate_;

  wavWriterOut.writeHeader(finalRiffChunk, WAV::stdFormatChunk, finalDataChunk);
}
