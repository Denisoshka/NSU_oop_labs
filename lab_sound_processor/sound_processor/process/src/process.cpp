#include <process.h>

#include "converters/include/converters.h"
#include "wav/include/wav.h"

// #include "converters/converter_tmp/converter_tmp.h"

conv::sampleBuffer convertFromWAVSampleBuffer(WAV::SampleBuffer &tmp) {
  return {tmp.get(), tmp.size(), tmp.size()};
}

process::process(const size_t sampleRate)
    : sampleRate_(sampleRate)
    , sample_(nullptr) {
  sample_ = std::make_unique<uint16_t[]>(sampleRate_);
}

void process::executeConversions(std::string &&FilePath) {
  FilePath_ = std::move(FilePath);
  if( FilePath_.find(".wav") == std::string::npos ) {
    throw;// todo make ex
  }

  FileOut_.open(FilePath_, std::ios_base::binary);
  if( FileOut_.fail() ) {
    throw;// todo make ex
  }

  conv::ConverterInterface interface;
  conv::sampleBuffer mainBufOut{};
  //  conv::sampleBuffer mainBuf;
  conv::sampleBuffer subBufIn{};


  WAV::WAVReader wavReaderSub;
  WAV::WAVReader wavReaderOut;
  WAV::WAVWriter wavWriterOut;
  WAV::SampleBuffer mainSampleOut{sampleRate_};
  WAV::SampleBuffer mainSampleIn{sampleRate_};
  WAV::SampleBuffer subSampleIn{sampleRate_};

  //  std::string task;
  //  size_t FileIn;
  //  size_t start;
  //  size_t end;

  // todo fix algo
  while( true ) {
    if( !interface.setTask() ) {
      break;
    }
    wavReaderSub.open(interface.getCurFile());

    while( !interface.taskFinished() ) {
      wavReaderSub.getSample(subSampleIn, interface.getCurSec());
      wavReaderOut.getSample(mainSampleOut, interface.getCurSec());

      mainBufOut = convertFromWAVSampleBuffer(mainSampleOut);

      interface.executeTask(mainBufOut, nullptr);
      wavWriterOut.writeSample(mainSampleOut, interface.getCurSec());
    }
  }
  wavWriterOut.writeHeader();
}
