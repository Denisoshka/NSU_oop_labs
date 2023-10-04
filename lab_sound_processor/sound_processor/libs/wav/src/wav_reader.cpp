#include "wav.h"
#include "wav_types.h"

WAV::WAVReader::WAVReader():dataStart_(0){
}

void WAV::WAVReader::open(std::string &&FilePath) {
  if (FilePath_ == FilePath){
    findData(DATA);
    return;
  }
  FilePath_ = std::move(FilePath);
  if( FilePath_.find(".wav") == std::string::npos ) {
    throw;// todo make ex
  }

  FileIn_.open(FilePath_, std::ios_base::binary);
  if( FileIn_.fail() ) {
    throw;// todo make ex
  }

  readHeader();

  findData(DATA);
}

void WAV::WAVReader::readHeader() {
  //  RIFFChunk headerRiff{};
  FileIn_.read((char *)&HeaderRiff_, sizeof(HeaderRiff_));
  dataStart_ += sizeof(HeaderRiff_);
  if( FileIn_.fail() ) {
    throw;// todo make ex
  }
  if( HeaderRiff_.Id != RIFF ) {
    throw;// todo make ex
  }
  if( HeaderRiff_.Format != WAVE ) {
    throw;// todo make ex
  }

  //  FormatChunk headerFormat{};
  FileIn_.read((char *)&HeaderFormat_, sizeof(HeaderFormat_));
  dataStart_ += sizeof(HeaderFormat_);
  if( FileIn_.fail() ) {
    throw;// todo make ex
  }
  if( HeaderFormat_.Id != FMT ) {
    throw;// todo make ex
  }
  if( HeaderFormat_.AudioFormat != AUDIO_FORMAT_PCM ) {
    throw;// todo make ex
  }
  if( HeaderFormat_.NumChannels != NUM_CHANNELS ) {
    throw;// todo make ex
  }
  if( HeaderFormat_.BitsPerSample != BITS_PER_SAMPLE ) {
    throw;// todo make ex
  }
  if( HeaderFormat_.SampleRate != SAMPLING_RATE ) {
    throw;// todo make ex
  }
}

void WAV::WAVReader::findData(uint32_t chunkId) {
  if (dataStart_){
    FileIn_.seekg(dataStart_, std::fstream::cur);
    return;
  }

  while( true ) {
    FileIn_.read((char *)&Data_, sizeof(Data_));
    dataStart_ += sizeof(Data_);
//    todo блять я чот сомнеавюсь что дата с этого момента начинается
    if( FileIn_.fail() ) {
      throw;// todo make ex
    }

    if( Data_.Id == chunkId ) {
      break;
    }

    FileIn_.seekg(Data_.Size, std::fstream::cur);
    dataStart_ += Data_.Size;
  }
}

void WAV::WAVReader::getSample(SampleBuffer &sample, size_t second) {
  FileIn_.seekg(second * sizeof(*sample.sample_), std::fstream::cur);
  FileIn_.read(reinterpret_cast<char *>(sample.sample_), sizeof(*sample.sample_) * sample.len_);
  if( FileIn_.fail() ) {
    throw;// todo make ex
  }
}

size_t WAV::WAVReader::getDuration() const {
  return Data_.Size / HeaderFormat_.BlockAlign;
}
