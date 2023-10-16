#include "wav.hpp"
#include "wav_exceptions.hpp"

namespace {
  static const int32_t kRIFF = 0x46464952;
  const int32_t kWAVE = 0x45564157;
  const int32_t kFMT = 0x20746d66;
  const int32_t kDATA = 0x61746164;

  const int16_t kAudioFormatPCM = 0x0001;
  const int16_t kNumChannels = 1;
  const int32_t kDefSampleRate = 44100;
  const int16_t kBitsPerByte = 8;
  const int16_t kBitsPerSample = kBitsPerByte * sizeof(int16_t);
  const int16_t kBlockAlign = kBitsPerSample * kNumChannels / kBitsPerByte;
  const int32_t kByteRate = kBlockAlign * kDefSampleRate;
  const int32_t kFormatChunkLen = 16;
  const int32_t kFinalRIFFChunkSizeWithoutDataSize = 36;

  struct __attribute__((__packed__)) RIFFChunk {
    int32_t Id;
    int32_t Size;
    int32_t Format;
  };

  struct __attribute__((__packed__)) FormatChunk {
    int32_t Id;
    int32_t Size;
    int16_t AudioFormat;
    int16_t NumChannels;
    int32_t SampleRate;
    int32_t ByteRate;
    int16_t BlockAlign;
    int16_t BitsPerSample;
  };

  struct __attribute__((__packed__)) DataChunk {
    int32_t Id;
    int32_t Size;
  };

  const RIFFChunk stdRIFFChunk{kRIFF, 0, kWAVE};
  const FormatChunk stdFormatChunk{kFMT,        kFormatChunkLen, kAudioFormatPCM, kNumChannels, kDefSampleRate, kByteRate,       kBlockAlign,     kBitsPerSample};
  const DataChunk stdDataChunk{kDATA, 0};
}// namespace

void WAV::makeWAVFile(const std::string& kFilePath) {
  std::ofstream stream{kFilePath};
}

WAV::WAVReader::WAVReader(const std::string& kFilePath) {
  open(kFilePath);
}

void WAV::WAVReader::open(const std::string& kFilePath) {
  FilePath_ = kFilePath;
  dataStart_ = 0;
  if( FileIn_.is_open() ) {
    FileIn_.close();
  }
  FileIn_.open(FilePath_, std::ios::in | std::ios::binary);
  if( FileIn_.fail() ) {
    throw StreamFailure(FilePath_);
  }

  readHeader();

  findData(kDATA);
}

void WAV::WAVReader::readHeader() {
  RIFFChunk HeaderRiff{};
  FileIn_.read(reinterpret_cast<char *>(&HeaderRiff), sizeof(HeaderRiff));
  dataStart_ += sizeof(HeaderRiff);

  if( HeaderRiff.Id != kRIFF ) {
    throw IncorrectRIFFHeader(FilePath_);
  }
  if( HeaderRiff.Format != kWAVE ) {
    throw IncorrectFormatType(FilePath_);
  }
  FormatChunk HeaderFormat{};
  FileIn_.read(reinterpret_cast<char *>(&HeaderFormat), sizeof(HeaderFormat));
  if( FileIn_.fail() ) {
    throw StreamFailure(FilePath_);
  }
  dataStart_ += sizeof(HeaderFormat);

  if( HeaderFormat.Id != kFMT ) {
    throw IncorrectAudioFormat(FilePath_);
  }
  if( HeaderFormat.AudioFormat != kAudioFormatPCM ) {
    throw IncorrectEncodingFormat(FilePath_);
  }
  if( HeaderFormat.NumChannels != kNumChannels ) {
    throw IncorrectChannelsNumber(FilePath_);
  }
  if( HeaderFormat.BitsPerSample != kBitsPerSample ) {
    throw IncorrectBitsPerSample(FilePath_);
  }
  if( HeaderFormat.SampleRate != kDefSampleRate ) {
    throw IncorrectSampleRate(FilePath_);
  }
  ByteRate_ = HeaderFormat.ByteRate;
}

void WAV::WAVReader::findData(const uint32_t kChunkId) {
  DataChunk Data{};
  while( !FileIn_.eof() ) {
    FileIn_.read(reinterpret_cast<char *>(&Data), sizeof(Data));
    if( FileIn_.fail() ) {
      throw StreamFailure(FilePath_);
    }
    dataStart_ += sizeof(Data);
    if( Data.Id == kChunkId ) {
      DataSize_ = Data.Size;
      return;
    }

    FileIn_.seekg(Data.Size, std::fstream::cur);
    if( FileIn_.fail() ) {
      throw StreamFailure(FilePath_);
    }
    dataStart_ += Data.Size;
  }
  throw ChunkNotFound(FilePath_, kChunkId);
}

void WAV::WAVReader::readSample(std::vector<int16_t>& kSample, const size_t kSecond) {
  FileIn_.seekg(dataStart_ + kSecond * kSample.size() * sizeof(*kSample.data()), std::fstream::beg);
  FileIn_.read(reinterpret_cast<char *>(kSample.data()), kSample.size() * sizeof(*kSample.data()));
  if( FileIn_.fail() ) {
    throw StreamFailure(FilePath_);
  }
}

size_t WAV::WAVReader::getDuration() const {
  return DataSize_ / ByteRate_;
}

WAV::WAVWriter::WAVWriter(const std::string& kFilePath) {
  open(kFilePath);
}

void WAV::WAVWriter::open(const std::string& kFilePath) {
  if( FileOut_.is_open() ) {
    FileOut_.close();
  }
  FilePath_ = kFilePath;
  FileOut_.open(FilePath_, std::ios::in | std::ios::out | std::ios::binary);
  if( FileOut_.fail() ) {
    throw StreamFailure(FilePath_);
  }
}

void WAV::WAVWriter::writeSample(const std::vector<int16_t>& kSample, const size_t kSecond) {
  FileOut_.seekp(dataStart_ + kSecond * kSample.size() * sizeof(*kSample.data()), std::ios::beg);
  FileOut_.write(reinterpret_cast<const char *>(kSample.data()),
                 kSample.size() * sizeof(*kSample.data()));
  FileOut_.flush();
  if( FileOut_.fail() ) {
    throw StreamFailure(FilePath_);
  }
}

void WAV::WAVWriter::writeHeader(const size_t kdataSize) {
  DataChunk finalDataChunk{stdDataChunk};
  finalDataChunk.Size = kdataSize * sizeof(uint16_t);
  RIFFChunk finalRiffChunk{stdRIFFChunk};
  finalRiffChunk.Size = kFinalRIFFChunkSizeWithoutDataSize + sizeof(uint16_t) * kdataSize;

  FileOut_.seekp(std::ios::beg);
  FileOut_.write(reinterpret_cast<const char *>(&finalRiffChunk), sizeof(finalRiffChunk));
  FileOut_.write(reinterpret_cast<const char *>(&stdFormatChunk), sizeof(stdFormatChunk));
  FileOut_.write(reinterpret_cast<const char *>(&finalDataChunk), sizeof(finalDataChunk));
  dataStart_ = sizeof(finalRiffChunk) + sizeof(stdFormatChunk) + sizeof(finalDataChunk);

  FileOut_.flush();
  if( FileOut_.fail() ) {
    throw StreamFailure(FilePath_);
  }
}
