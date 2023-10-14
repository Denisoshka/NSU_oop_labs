#include "wav.hpp"
#include "wav_exceptions.hpp"

WAV::WAVReader::WAVReader(const std::string& FilePath) {
  open(FilePath);
}

void WAV::WAVReader::open(const std::string& FilePath) {
  FilePath_ = FilePath;
  dataStart_ = 0;
  if (FileIn_.is_open()){
    FileIn_.close();
  }
  FileIn_.open(FilePath_, std::ios::in | std::ios::binary);
  if( FileIn_.fail() ) {
    throw StreamFailure(FilePath_);
  }

  readHeader();

  findData(DATA);
}

void WAV::WAVReader::readHeader() {
  FileIn_.read(reinterpret_cast<char*>(&HeaderRiff_), sizeof(HeaderRiff_));
  dataStart_ += sizeof(HeaderRiff_);

  if( HeaderRiff_.Id != RIFF ) {
    throw IncorrectRIFFHeader(FilePath_);
  }
  if( HeaderRiff_.Format != WAVE ) {
    throw IncorrectFormatType(FilePath_);
  }

  FileIn_.read(reinterpret_cast<char*>(&HeaderFormat_), sizeof(HeaderFormat_));
  if( FileIn_.fail() ) {
    throw StreamFailure(FilePath_);
  }
  dataStart_ += sizeof(HeaderFormat_);

  if( HeaderFormat_.Id != FMT ) {
    throw IncorrectAudioFormat(FilePath_);
  }
  if( HeaderFormat_.AudioFormat != AUDIO_FORMAT_PCM ) {
    throw IncorrectEncodingFormat(FilePath_);
  }
  if( HeaderFormat_.NumChannels != NUM_CHANNELS ) {
    throw IncorrectChannelsNumber(FilePath_);
  }
  if( HeaderFormat_.BitsPerSample != BITS_PER_SAMPLE ) {
    throw IncorrectBitsPerSample(FilePath_);
  }
  if( HeaderFormat_.SampleRate != SAMPLE_RATE ) {
    throw IncorrectSampleRate(FilePath_);
  }
}

void WAV::WAVReader::findData(uint32_t chunkId) {
  while( !FileIn_.eof() ) {
    FileIn_.read(reinterpret_cast<char*>(&Data_), sizeof(Data_));
    if( FileIn_.fail() ) {
      throw StreamFailure(FilePath_);
    }
    dataStart_ += sizeof(Data_);
    if( Data_.Id == chunkId ) {
      return;
    }

    FileIn_.seekg(Data_.Size, std::fstream::cur);
    if( FileIn_.fail() ) {
      throw StreamFailure(FilePath_);
    }
    dataStart_ += Data_.Size;
  }
  throw ChunkNotFound(FilePath_, chunkId);
}

void WAV::WAVReader::readSample(std::vector<int16_t>& sample, const size_t second) {
  FileIn_.seekg(dataStart_ + second * sample.size() * sizeof(*sample.data()), std::fstream::beg);
  FileIn_.read(reinterpret_cast<char*>(sample.data()), sample.size() * sizeof(*sample.data()));
  if( FileIn_.fail() ) {
    throw StreamFailure(FilePath_);
  }
}

size_t WAV::WAVReader::getDuration() const {
  return Data_.Size / HeaderFormat_.ByteRate;
}
