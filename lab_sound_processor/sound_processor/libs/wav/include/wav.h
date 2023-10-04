#ifndef WAV_WAV_H
#define WAV_WAV_H

#include <fstream>
#include <string>
#include <memory>

namespace WAV {
  struct RIFFChunk {
    uint32_t Id;
    uint32_t Size;
    uint32_t Format;
  };

  struct DataChunk {
    uint32_t Id;
    uint32_t Size;
  };

  struct FormatChunk {
    uint32_t Id;
    uint32_t Size;
    uint16_t AudioFormat;
    uint16_t NumChannels;
    uint32_t SampleRate;
    uint32_t ByteRate;
    uint16_t BlockAlign;
    uint16_t BitsPerSample;
  };

  class SampleBuffer {
  public:
    explicit SampleBuffer(size_t sampleRate);
    uint16_t *get();
    [[nodiscard]] size_t size() const;
  private:
    std::unique_ptr< uint16_t[]> sample_;
    size_t curLen_;
    size_t len_;
  };

  class WAVReader {
  public:
    WAVReader();
    void open(std::string &&FilePath);
    void getSample(SampleBuffer &sample, size_t second);
    size_t getDuration() const;

  private:
    void readHeader();
    void findData(uint32_t chunkId);

    RIFFChunk HeaderRiff_{};
    FormatChunk HeaderFormat_{};
    DataChunk Data_{};
    std::ifstream FileIn_;
    std::string FilePath_;
    size_t dataStart_;
  };

  class WAVWriter {
  public:
    void open(std::string &&FilePath);
    //  void readWav(std::string &&FilePath);
    void writeSample(SampleBuffer &sample, size_t second);
    void writeHeader();

  private:
    std::string FilePath_;
    std::ofstream FileOut_;
    size_t dataStart_;
  };
}// namespace WAV
#endif// WAV_WAV_H
