#ifndef WAV_WAV_EXCEPTIONS_H
#define WAV_WAV_EXCEPTIONS_H

#include <ios>
#include <stdexcept>
namespace WAV {
  class IncorrectFileFormat: public std::invalid_argument {
  protected:
    explicit IncorrectFileFormat(const std::string& filePath, const std::string& desc);
  };

  class IncorrectExtension: public IncorrectFileFormat {
  public:
    explicit IncorrectExtension(const std::string& filePath);
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

  class StreamFailure: public std::ios_base::failure {
  public:
    explicit StreamFailure(const std::string& filePath);
  };

  class IncorrectEncodingFormat: public IncorrectFileFormat {
  public:
    explicit IncorrectEncodingFormat(const std::string& file_path);
  };
}// namespace WAV
#endif// WAV_WAV_EXCEPTIONS_H
