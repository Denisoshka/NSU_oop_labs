#include "wav.h"
#include "wav_types.h"

void wav::readWav(std::string &&FilePath) {
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

void wav::readHeader() {
//  RIFFChunk headerRiff{};
  FileIn_.read((char *)&HeaderRiff_, sizeof(HeaderRiff_));
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

void wav::findData(uint32_t chunkId) {
  while( true ) {
    FileIn_.read((char *)&Data_, sizeof(Data_));
    if( FileIn_.fail() ) {
      throw;// todo make ex
    }

    if( Data_.Id == chunkId ) {
      break;
    }

    FileIn_.seekg(Data_.Size, std::fstream::cur);
  }
}
