#include "wav.hpp"
#include "wav_exceptions.hpp"

WAV::WAVWriter::WAVWriter(const std::string& FilePath) {
  open(FilePath);
}

void WAV::WAVWriter::open(const std::string& FilePath) {
  if( FileOut_.is_open() ) {
    FileOut_.close();
  }
  FilePath_ = FilePath;
  FileOut_.open(FilePath_, std::ios::in | std::ios::out | std::ios::binary);
  if( FileOut_.fail() ) {
    throw StreamFailure(FilePath_);
  }
}

void WAV::WAVWriter::writeSample(const std::vector<int16_t>& sample, const size_t second) {
  FileOut_.seekp(dataStart_ + second * sample.size() * sizeof(*sample.data()), std::ios::beg);
  FileOut_.write(reinterpret_cast<const char*>(sample.data()),
                 sample.size() * sizeof(*sample.data()));
  FileOut_.flush();
  if( FileOut_.fail() ) {
    throw StreamFailure(FilePath_);
  }
}

void WAV::WAVWriter::writeHeader(const RIFFChunk& riffChunk, const FormatChunk& formatChunk,
                                 const DataChunk& dataChunk) {
  FileOut_.seekp(std::ios::beg);
  FileOut_.write(reinterpret_cast<const char*>(&riffChunk), sizeof(riffChunk));
  FileOut_.write(reinterpret_cast<const char*>(&formatChunk), sizeof(formatChunk));
  FileOut_.write(reinterpret_cast<const char*>(&dataChunk), sizeof(dataChunk));
  dataStart_ = sizeof(formatChunk) + sizeof(riffChunk) + sizeof(dataChunk);

  FileOut_.flush();
  if( FileOut_.fail() ) {
    throw StreamFailure(FilePath_);
  }
}
