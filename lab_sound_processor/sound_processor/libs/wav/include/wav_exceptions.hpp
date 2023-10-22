#pragma once

#include <ios>
#include <stdexcept>

namespace {

}// namespace

namespace WAV {
  enum WAVExceptionsCode: size_t {
    ekBaseWAVExceptionsCode = 45'564'157'0,
    ekIncorrectRIFFHeader,
    ekIncorrectFormatType,
    ekIncorrectFormatData,
    ekIncorrectAudioFormat,
    ekIncorrectChannelsNumber,
    ekIncorrectBitsPerSample,
    ekIncorrectSampleRate,
    ekChunkNotFound,
    ekStreamFailure,
    ekIncorrectEncodingFormat,
  };

  class WAVEexception {
  public:
    WAVEexception(WAVExceptionsCode code);
    [[nodiscard]] WAVExceptionsCode getErrorCode() const;
    [[nodiscard]] virtual const char *what() const = 0;

  protected:
    WAVExceptionsCode ErrorCode_;
  };

  class IncorrectFileFormat: public std::invalid_argument,
                             virtual public WAVEexception {
  public:
    explicit IncorrectFileFormat(const std::string& filePath, const std::string& desc);
    [[nodiscard]] const char *what() const noexcept override;
  };

  class IncorrectRIFFHeader: public IncorrectFileFormat {
  public:
    explicit IncorrectRIFFHeader(const std::string& filePath);
  };

  class IncorrectFormatType: public IncorrectFileFormat {
  public:
    explicit IncorrectFormatType(const std::string& filePath);
  };

  class IncorrectFormatData: public IncorrectFileFormat {
  public:
    explicit IncorrectFormatData(const std::string& filePath);
  };

  class IncorrectAudioFormat: public IncorrectFileFormat {
  public:
    explicit IncorrectAudioFormat(const std::string& filePath);
  };

  class IncorrectChannelsNumber: public IncorrectFileFormat {
  public:
    explicit IncorrectChannelsNumber(const std::string& filePath);
  };

  class IncorrectBitsPerSample: public IncorrectFileFormat {
  public:
    explicit IncorrectBitsPerSample(const std::string& filePath);
  };

  class IncorrectSampleRate: public IncorrectFileFormat {
  public:
    explicit IncorrectSampleRate(const std::string& filePath);
  };

  class ChunkNotFound: public IncorrectFileFormat {
  public:
    explicit ChunkNotFound(const std::string& filePath, uint32_t chunk_ID);
  };

  class StreamFailure: public WAVEexception,
                       public std::ios_base::failure {
  public:
    explicit StreamFailure(const std::string& filePath);
    [[nodiscard]] const char *what() const noexcept override;
  };

  class IncorrectEncodingFormat: public IncorrectFileFormat {
  public:
    explicit IncorrectEncodingFormat(const std::string& file_path);
  };
}// namespace WAV
