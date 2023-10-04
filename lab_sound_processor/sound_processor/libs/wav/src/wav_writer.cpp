#include "wav.h"

void WAV::WAVWriter::open(std::string && FilePath) {
  if( FilePath_ == FilePath ) {
    return;
  }
  FilePath_ = std::move(FilePath);
  FileOut_.open(FilePath_, std::ios_base::binary);
  if( FileOut_.fail() ) {
    throw;// todo make ex
  }

}

void WAV::WAVWriter::writeSample(SampleBuffer &sample, size_t second) {
  FileOut_.seekp((dataStart_ + second) * sizeof(*sample.sample_), std::ios_base::beg);
  FileOut_.write(reinterpret_cast<char *>(sample.sample_), sizeof(*sample.sample_) * sample.len_);
}
