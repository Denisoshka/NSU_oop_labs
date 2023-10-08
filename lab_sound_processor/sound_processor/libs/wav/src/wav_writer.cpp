#include "wav.hpp"
#include "wav_exceptions.hpp"

WAV::WAVWriter::WAVWriter(std::string &FilePath) {
  open(FilePath);
  FilePath_ = FilePath;
}

WAV::WAVWriter::~WAVWriter() {
  FileOut_.close();
}

void WAV::WAVWriter::open(std::string &FilePath) {
  if( FilePath_ == FilePath ) {
    return;
  }
  if( FilePath_.find(".wav") == std::string::npos ) {
    throw IncorrectExtension(FilePath);
  }
  FileOut_.open(FilePath_, std::ios_base::binary);
  if( FileOut_.fail() ) {
    throw StreamFailure(FilePath_);
  }
}

void WAV::WAVWriter::writeSample(SampleBuffer &sample, size_t second) {
  if( FileOut_.seekp(dataStart_ + second * sample.size() * sizeof(*sample.get()),
                     std::ios_base::beg)
              .fail() ) {
    throw StreamFailure(FilePath_);
  }
  if( FileOut_.write(reinterpret_cast<char *>(sample.get()), sample.size() * sizeof(*sample.get()))
              .fail() ) {
    throw StreamFailure(FilePath_);
  }
}

void WAV::WAVWriter::writeHeader(RIFFChunk &riffChunk, FormatChunk &formatChunk,
                                 DataChunk &dataChunk) {
  FileOut_.seekp(std::ios_base::beg);
  dataStart_ = 0;
  if( FileOut_.write(reinterpret_cast<char *>(&riffChunk), sizeof(riffChunk)).fail() ) {
    throw StreamFailure(FilePath_);
  }
  dataStart_ += sizeof(riffChunk);

  if( FileOut_.write(reinterpret_cast<char *>(&formatChunk), sizeof(formatChunk)).fail() ) {
    throw StreamFailure(FilePath_);
  }
  dataStart_ += sizeof(formatChunk);

  if( FileOut_.write(reinterpret_cast<char *>(&dataChunk), sizeof(dataChunk)).fail() ) {
    throw StreamFailure(FilePath_);
  }
  dataStart_ += sizeof(dataChunk);
}
