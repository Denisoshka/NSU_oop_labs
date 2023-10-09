#include "wav.hpp"
#include "wav_exceptions.hpp"

WAV::WAVWriter::WAVWriter(const std::string& FilePath) {
  open(FilePath);
}

void WAV::WAVWriter::open(const std::string& FilePath) {
  if( FilePath_ == FilePath ) {
    return;
  }
  if( FilePath.find(".wav") == std::string::npos ) {
    throw IncorrectExtension(FilePath);
  }
  FilePath_ = FilePath;
  FileOut_.open(FilePath_, std::ios::in | std::ios::out | std::ios::binary);
  if( FileOut_.fail() ) {
    throw StreamFailure(FilePath_);
  }
}

void WAV::WAVWriter::writeSample(const std::vector<int16_t>& sample, const size_t second) {
  FileOut_.seekp(dataStart_ + second * sample.size() * sizeof(*sample.data()), std::ios::beg);
  FileOut_.flush();
  if( FileOut_.fail() ) {
    throw StreamFailure(FilePath_);
  }
  FileOut_.write(reinterpret_cast<const char*>(sample.data()),
                 sample.size() * sizeof(*sample.data()));
  if( FileOut_.fail() ) {
    throw StreamFailure(FilePath_);
  }
  if( FileOut_.fail() ) {
    throw StreamFailure(FilePath_);
  }
}

void WAV::WAVWriter::writeHeader(const RIFFChunk& riffChunk, const FormatChunk& formatChunk,
                                 const DataChunk& dataChunk) {
  FileOut_.seekp(std::ios::beg);
  dataStart_ = 0;
  FileOut_.write(reinterpret_cast<const char*>(&riffChunk), sizeof(riffChunk));
  if( FileOut_.fail() ) {
    throw StreamFailure(FilePath_);
  }
  dataStart_ += sizeof(riffChunk);

  FileOut_.write(reinterpret_cast<const char*>(&formatChunk), sizeof(formatChunk));
  if( FileOut_.fail() ) {
    throw StreamFailure(FilePath_);
  }
  dataStart_ += sizeof(formatChunk);

  FileOut_.write(reinterpret_cast<const char*>(&dataChunk), sizeof(dataChunk));
  if( FileOut_.fail() ) {
    throw StreamFailure(FilePath_);
  }
  dataStart_ += sizeof(dataChunk);

  FileOut_.flush();
  if( FileOut_.fail() ) {
    throw StreamFailure(FilePath_);
  }
}
