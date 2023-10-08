#ifndef WAV_WAV_HPP
#define WAV_WAV_HPP

#include <array>
#include <cstdint>
#include <fstream>
#include <memory>
#include <string>

namespace WAV {
  const uint32_t RIFF = 0x46464952;
  const uint32_t WAVE = 0x45564157;
  const uint32_t FMT = 0x20746d66;
  const uint32_t DATA = 0x61746164;

  const uint16_t AUDIO_FORMAT_PCM = 0x0001;
  const uint16_t NUM_CHANNELS = 1;
  const uint32_t SAMPLE_RATE = 44100;
  const uint16_t BITS_PER_BYTE = 8;
  const uint16_t BITS_PER_SAMPLE = BITS_PER_BYTE * sizeof(int16_t);
  const uint16_t BLOCK_ALIGN = BITS_PER_SAMPLE * NUM_CHANNELS / BITS_PER_BYTE;
  const uint32_t BYTE_RATE = BLOCK_ALIGN * SAMPLE_RATE;
  const uint32_t FORMAT_CHUNK_LEN = 16;

  struct __attribute__((__packed__)) RIFFChunk {
    uint32_t Id;
    uint32_t Size;
    uint32_t Format;
  };

  struct __attribute__((__packed__)) FormatChunk {
    uint32_t Id;
    uint32_t Size;
    uint16_t AudioFormat;
    uint16_t NumChannels;
    uint32_t SampleRate;
    uint32_t ByteRate;
    uint16_t BlockAlign;
    uint16_t BitsPerSample;
  };

  struct __attribute__((__packed__)) DataChunk {
    uint32_t Id;
    uint32_t Size;
  };

  RIFFChunk stdRIFFChunk{RIFF, 0, WAVE};
  FormatChunk stdFormatChunk{FMT,         FORMAT_CHUNK_LEN, AUDIO_FORMAT_PCM, NUM_CHANNELS,
                             SAMPLE_RATE, BYTE_RATE,        BLOCK_ALIGN,      BITS_PER_SAMPLE};
  DataChunk stdDataChunk{DATA, 0};

  class SampleBuffer {
  public:
    explicit SampleBuffer(size_t sampleRate);
    uint16_t *get();
    [[nodiscard]] size_t size() const;

  private:
    std::unique_ptr<uint16_t[]> sample_;
    size_t curLen_;
    size_t len_;
  };

  class WAVReader {
  public:
    WAVReader() = default;
    WAVReader(std::string &FilePath);
    ~WAVReader();
    void open(std::string &FilePath);
    void readSample(SampleBuffer &sample, size_t second);
    size_t getDuration() const;

  private:
    void readHeader();
    void findData(uint32_t chunkId);

    RIFFChunk HeaderRiff_{};
    FormatChunk HeaderFormat_{};
    DataChunk Data_{};
    std::ifstream FileIn_;
    std::string FilePath_;
    size_t dataStart_ = 0;
  };

  class WAVWriter {
  public:
    WAVWriter() = default;
    WAVWriter(std::string &FilePath);
    ~WAVWriter();
    void open(std::string &FilePath);
    void writeSample(SampleBuffer &sample, size_t second);
    void writeHeader(RIFFChunk &riffChunk, FormatChunk &formatChunk, DataChunk &dataChunk);

  private:
    std::string FilePath_;
    std::fstream FileOut_;
    size_t dataStart_ = 0;
    // todo add data start
  };
}// namespace WAV
#endif// WAV_WAV_HPP
