#include "wav.hpp"
#include "wav_exceptions.hpp"

WAV::WAVWriter::WAVWriter(const std::string& FilePath) {
  open(FilePath);
  FilePath_ = FilePath;
}

void WAV::WAVWriter::open(const std::string& FilePath) {
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

void WAV::WAVWriter::writeSample(const std::vector<int16_t>& sample, const size_t second) {
  if( FileOut_.seekp(dataStart_ + second * sample.size() * sizeof(*sample.data()),
                     std::ios_base::beg)
              .fail() ) {
    throw StreamFailure(FilePath_);
  }
  if( FileOut_.write(reinterpret_cast<const char*>(sample.data()),
                     sample.size() * sizeof(*sample.data()))
              .fail() ) {
    throw StreamFailure(FilePath_);
  }
}

void WAV::WAVWriter::writeHeader(const RIFFChunk& riffChunk, const FormatChunk& formatChunk,
                                 const DataChunk& dataChunk) {
  FileOut_.seekp(std::ios_base::beg);
  dataStart_ = 0;
  if( FileOut_.write(reinterpret_cast<const char*>(&riffChunk), sizeof(riffChunk)).fail() ) {
    throw StreamFailure(FilePath_);
  }
  dataStart_ += sizeof(riffChunk);

  if( FileOut_.write(reinterpret_cast<const char*>(&formatChunk), sizeof(formatChunk)).fail() ) {
    throw StreamFailure(FilePath_);
  }
  dataStart_ += sizeof(formatChunk);

  if( FileOut_.write(reinterpret_cast<const char*>(&dataChunk), sizeof(dataChunk)).fail() ) {
    throw StreamFailure(FilePath_);
  }
  dataStart_ += sizeof(dataChunk);
}
