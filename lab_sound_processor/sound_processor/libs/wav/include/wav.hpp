#ifndef WAV_WAV_HPP
#define WAV_WAV_HPP

#include <array>
#include <cstdint>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

namespace WAV {
  const int32_t RIFF = 0x46464952;
  const int32_t WAVE = 0x45564157;
  const int32_t FMT = 0x20746d66;
  const int32_t DATA = 0x61746164;

  const int16_t AUDIO_FORMAT_PCM = 0x0001;
  const int16_t NUM_CHANNELS = 1;
  const int32_t SAMPLE_RATE = 44100;
  const int16_t BITS_PER_BYTE = 8;
  const int16_t BITS_PER_SAMPLE = BITS_PER_BYTE * sizeof(int16_t);
  const int16_t BLOCK_ALIGN = BITS_PER_SAMPLE * NUM_CHANNELS / BITS_PER_BYTE;
  const int32_t BYTE_RATE = BLOCK_ALIGN * SAMPLE_RATE;
  const int32_t FORMAT_CHUNK_LEN = 16;
  const int32_t FINAL_RIFF_CHUNK_SIZE_WITHOUT_DATA_SIZE = 36;

  /*
         int a = (sizeof(WAV::stdRIFFChunk)
           - (sizeof(WAV::stdRIFFChunk.Id) + sizeof(WAV::stdRIFFChunk.Size)))
          + sizeof(WAV::stdFormatChunk) + sizeof(WAV::DataChunk);
*/

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

  const RIFFChunk stdRIFFChunk{RIFF, 0, WAVE};
  const FormatChunk stdFormatChunk{FMT,          FORMAT_CHUNK_LEN, AUDIO_FORMAT_PCM,
                                   NUM_CHANNELS, SAMPLE_RATE,      BYTE_RATE,
                                   BLOCK_ALIGN,  BITS_PER_SAMPLE};
  const DataChunk stdDataChunk{DATA, 0};

  class WAVReader {
  public:
    WAVReader() = default;
    WAVReader(std::string& FilePath);
    ~WAVReader() = default;
    void open(const std::string& FilePath);
    void readSample(std::vector<int16_t>& sample, const size_t second);
    //    FormatChunk HeaderFormat_{.ByteRate = 1};
    size_t getDuration() const;

  private:
    void readHeader();
    void findData(uint32_t chunkId);

    RIFFChunk HeaderRiff_{};
    FormatChunk HeaderFormat_{.ByteRate = 1};

    DataChunk Data_{};
    std::fstream FileIn_;
    std::string FilePath_;
    size_t dataStart_ = 0;
  };

  class WAVWriter {
  public:
    WAVWriter() = default;
    WAVWriter(const std::string& FilePath);
    ~WAVWriter() = default;
    void open(const std::string& FilePath);
    void writeSample(const std::vector<int16_t>& sample, const size_t second);
    void writeHeader(const RIFFChunk& riffChunk, const FormatChunk& formatChunk,
                     const DataChunk& dataChunk);

  private:
    std::string FilePath_;
    std::fstream FileOut_;
    size_t dataStart_ = 0;
    size_t writedData_ = 0;
  };
}// namespace WAV
#endif// WAV_WAV_HPP
