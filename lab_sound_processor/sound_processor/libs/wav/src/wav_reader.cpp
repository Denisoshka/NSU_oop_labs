#include "wav.hpp"
#include "wav_exceptions.hpp"

WAV::WAVReader::WAVReader(std::string& FilePath) {
  open(FilePath);
  FilePath_ = FilePath;
}

void WAV::WAVReader::open(const std::string& FilePath) {
  if( FilePath_ == FilePath ) {
    findData(DATA);
    return;
  }

  FilePath_ = FilePath;
  if( FilePath_.find(".wav") == std::string::npos ) {
    throw IncorrectExtension(FilePath_);
  }

  FileIn_.open(FilePath_, std::ios::in | std::ios_base::binary);
  if( FileIn_.fail() ) {
    throw StreamFailure(FilePath_);
  }

  readHeader();

  findData(DATA);
}

void WAV::WAVReader::readHeader() {
  FileIn_.read(reinterpret_cast<char*>(&HeaderRiff_), sizeof(HeaderRiff_));
  dataStart_ += sizeof(HeaderRiff_);
  if( FileIn_.fail() ) {
    throw StreamFailure(FilePath_);
  }
  if( HeaderRiff_.Id != RIFF ) {
    throw IncorrectRIFFHeader(FilePath_);
  }
  if( HeaderRiff_.Format != WAVE ) {
    throw IncorrectFormatType(FilePath_);
  }

  FileIn_.read(reinterpret_cast<char*>(&HeaderFormat_), sizeof(HeaderFormat_));
  dataStart_ += sizeof(HeaderFormat_);
  if( FileIn_.fail() ) {
    throw StreamFailure(FilePath_);
  }
  if( HeaderFormat_.Id != FMT ) {
    throw;// todo make ex
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
  if( dataStart_ ) {
    if( FileIn_.seekg(dataStart_, std::fstream::cur).fail() ) {
      throw StreamFailure(FilePath_);
    }
    return;
  }

  while( !FileIn_.eof() ) {
    if( FileIn_.read(reinterpret_cast<char*>(&Data_), sizeof(Data_)).fail() ) {
      throw StreamFailure(FilePath_);
    }
    dataStart_ += sizeof(Data_);
    //    todo я чот сомнеавюсь что дата с этого момента начинается
    if( Data_.Id == chunkId ) {
      return;
    }

    FileIn_.seekg(Data_.Size, std::fstream::cur);
    dataStart_ += Data_.Size;
  }
  throw ChunkNotFound(FilePath_, chunkId);
}

void WAV::WAVReader::readSample(std::vector<int16_t>& sample, const size_t second) {
  if( FileIn_.seekg(dataStart_ + second * sample.size() * sizeof(*sample.data()), std::fstream::cur)
              .fail() ) {
    throw StreamFailure(FilePath_);
  }
  if( FileIn_.read(reinterpret_cast<char*>(sample.data()), sample.size() * sizeof(*sample.data()))
              .fail() ) {
    throw StreamFailure(FilePath_);
  }
}

size_t WAV::WAVReader::getDuration() const {
  return Data_.Size / HeaderFormat_.BlockAlign;
}
