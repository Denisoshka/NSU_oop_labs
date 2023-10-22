#include "wav_exceptions.hpp"

namespace WAV {
  WAVEexception::WAVEexception(WAVExceptionsCode code)
      : ErrorCode_(code) {
  }

  IncorrectFileFormat::IncorrectFileFormat(const std::string& filePath, const std::string& desc)
      : std::invalid_argument(filePath + ": Incorrect file format : " + desc)
      , WAVEexception(ekBaseWAVExceptionsCode) {
  }

  const char *IncorrectFileFormat::what() const noexcept {
    return std::invalid_argument::what();
  }

  WAVExceptionsCode WAVEexception::getErrorCode() const {
    return ErrorCode_;
  }

  IncorrectRIFFHeader::IncorrectRIFFHeader(const std::string& filePath)
      : IncorrectFileFormat(filePath, "Incorrect kRIFF header")
      , WAVEexception(ekIncorrectRIFFHeader) {
  }

  IncorrectFormatType::IncorrectFormatType(const std::string& filePath)
      : IncorrectFileFormat(filePath, "Incorrect format type")
      , WAVEexception(ekIncorrectFormatType) {
  }

  IncorrectFormatData::IncorrectFormatData(const std::string& filePath)
      : IncorrectFileFormat(filePath, "Reading format data error")
      , WAVEexception(ekIncorrectFormatData) {
  }

  IncorrectAudioFormat::IncorrectAudioFormat(const std::string& filePath)
      : IncorrectFileFormat(filePath, "Incorrect audio format")
      , WAVEexception(ekIncorrectAudioFormat) {
  }

  IncorrectChannelsNumber::IncorrectChannelsNumber(const std::string& filePath)
      : IncorrectFileFormat(filePath, "Incorrect channels number")
      , WAVEexception(ekIncorrectChannelsNumber) {
  }

  IncorrectBitsPerSample::IncorrectBitsPerSample(const std::string& filePath)
      : IncorrectFileFormat(filePath, "Incorrect bits quantity per sample")
      , WAVEexception(ekIncorrectBitsPerSample) {
  }

  IncorrectSampleRate::IncorrectSampleRate(const std::string& filePath)
      : IncorrectFileFormat(filePath, "Incorrect sample rate")
      , WAVEexception(ekIncorrectSampleRate) {
  }

  ChunkNotFound::ChunkNotFound(const std::string& filePath, uint32_t chunk_ID)
      : IncorrectFileFormat(filePath, "Searching chunk " + std::to_string(chunk_ID) + " error")
      , WAVEexception(ekChunkNotFound) {
  }

  StreamFailure::StreamFailure(const std::string& filePath)
      : std::ios_base::failure(filePath + " failure ")
      , WAVEexception(ekStreamFailure) {
  }

  const char *StreamFailure::what() const noexcept {
    return std::ios_base::failure::what();
  }

  IncorrectEncodingFormat::IncorrectEncodingFormat(const std::string& file_path)
      : IncorrectFileFormat(file_path, "Incorrect encoding ")
      , WAVEexception(ekIncorrectEncodingFormat) {
  }
}// namespace WAV
