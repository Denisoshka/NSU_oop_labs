#pragma once

#include <fstream>
#include <memory>
#include <string>
#include <vector>

namespace WAV {
  class WAVReader {
  public:
    WAVReader() = default;
    WAVReader(const std::string& kFilePath);
    ~WAVReader() = default;
    void open(const std::string& kFilePath);
    void readSample(std::vector<int16_t>& kSample, const size_t kSecond);
    size_t getDuration() const;

  private:
    void readHeader();
    void findDATA();
    void readFormat();
    size_t DataSize_ = 0;
    size_t ByteRate_ = 1;
    std::fstream FileIn_;
    std::string FilePath_;
    size_t dataStart_ = 0;
  };

  class WAVWriter {
  public:
    WAVWriter() = default;
    WAVWriter(const std::string& kFilePath);
    ~WAVWriter() = default;
    void open(const std::string& kFilePath);
    void writeSample(const std::vector<int16_t>& kSample, const size_t kSecond);
    void writeHeader(const size_t kDuration);

  private:
    std::string FilePath_;
    std::fstream FileOut_;
    size_t dataStart_ = 0;
    size_t writedData_ = 0;
  };

}// namespace WAV
