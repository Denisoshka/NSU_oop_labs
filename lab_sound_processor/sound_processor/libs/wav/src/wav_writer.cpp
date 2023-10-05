#include "wav.h"

WAV::WAVWriter::WAVWriter(std::string &FilePath) {
  FilePath_ = FilePath;
  if( FilePath_.find(".wav") == std::string::npos ) {
    throw;// todo make ex
  }

  FileOut_.open(FilePath_, std::ios_base::binary);
  if( FileOut_.fail() ) {
    throw;// todo make ex
  }

}

void WAV::WAVWriter::open(std::string &FilePath) {
  if( FilePath_ == FilePath ) {
    return;
  }
  FilePath_ = FilePath;
  FileOut_.open(FilePath_, std::ios_base::binary);
  if( FileOut_.fail() ) {
    throw;// todo make ex
  }
}

void WAV::WAVWriter::writeSample(SampleBuffer &sample, size_t second) {
  FileOut_.seekp((dataStart_ + second) * sizeof(*sample.get()), std::ios_base::beg);
  FileOut_.write(reinterpret_cast<char *>(sample.get()), sizeof(*sample.get()) * sample.size());
}
