#ifndef WAV_WAV_H
#define WAV_WAV_H

#include <fstream>
#include <string>

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

class WAVReader {
public:
  void readWav(std::string &&FilePath);
  void readHeader();
  void findData(uint32_t chunkId);
  void getSample(uint16_t *SampleBuffer, size_t SampleBufferLen, size_t start, size_t end);

private:
  RIFFChunk HeaderRiff_{};
  FormatChunk HeaderFormat_{};
  DataChunk Data_{};
  std::ifstream FileIn_;
  std::string FilePath_;
};

class WAVWriter {
public:
//  void readWav(std::string &&FilePath);
  void writeSample(uint16_t *sampleBuffer, size_t sampleBufferLen, size_t start, size_t end);
  void writeHeader();
private:
  std::ofstream FileOut_;
  size_t dataStart_;
};
#endif// WAV_WAV_H
