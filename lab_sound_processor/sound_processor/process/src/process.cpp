#include <process.h>

#include "converters/include/converters.h"
#include "wav/include/wav.h"

// #include "converters/converter_tmp/converter_tmp.h"

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

  ConverterInterface interface;
  WAVReader wavReader;
  WAVWriter wavWriter;
  std::string task;
  size_t FileIn;
  size_t start;
  size_t end;
//todo fix algo
  while( !interface.getTask(FileIn, start, end).empty() ) {
    interface.getTask(FileIn, start, end);
    while(!interface.taskFinished()) {
      wavReader.getSample(sample_.get(), sampleRate_, start, end);
      interface.executeTask(sample_.get(), sampleRate_);
      wavWriter.writeSample(sample_.get(), sampleRate_, start, end);
    }
  }
  wavWriter.writeHeader();
}
