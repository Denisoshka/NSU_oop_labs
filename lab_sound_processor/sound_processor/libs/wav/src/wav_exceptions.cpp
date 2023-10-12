#include "wav_exceptions.hpp"

WAV::IncorrectFileFormat::IncorrectFileFormat(const std::string& filePath, const std::string& desc)
    : std::invalid_argument(filePath + ": Incorrect file format : " + desc) {
}

WAV::IncorrectExtension::IncorrectExtension(const std::string& filePath)
    : IncorrectFileFormat(filePath, "Incorrect file extension") {
}

WAV::IncorrectRIFFHeader::IncorrectRIFFHeader(const std::string& filePath)
    : IncorrectFileFormat(filePath, "Incorrect RIFF header") {
}

WAV::IncorrectFormatType::IncorrectFormatType(const std::string& filePath)
    : IncorrectFileFormat(filePath, "Incorrect format type") {
}

WAV::IncorrectFormatData::IncorrectFormatData(const std::string& filePath)
    : IncorrectFileFormat(filePath, "Reading format data error") {
}

WAV::IncorrectAudioFormat::IncorrectAudioFormat(const std::string& filePath)
    : IncorrectFileFormat(filePath, "Incorrect audio format") {
}

WAV::IncorrectChannelsNumber::IncorrectChannelsNumber(const std::string& filePath)
    : IncorrectFileFormat(filePath, "Incorrect channels number") {
}

WAV::IncorrectBitsPerSample::IncorrectBitsPerSample(const std::string& filePath)
    : IncorrectFileFormat(filePath, "Incorrect bits quantity per sample") {
}

WAV::IncorrectSampleRate::IncorrectSampleRate(const std::string& filePath)
    : IncorrectFileFormat(filePath, "Incorrect sample rate") {
}

WAV::ChunkNotFound::ChunkNotFound(const std::string& filePath, uint32_t chunk_ID)
    : IncorrectFileFormat(filePath, "Searching chunk " + std::to_string(chunk_ID) + " error") {
}

WAV::StreamFailure::StreamFailure(const std::string& filePath)
    : std::ios_base::failure(filePath + " failure ") {
}

WAV::IncorrectEncodingFormat::IncorrectEncodingFormat(const std::string& file_path)
    : IncorrectFileFormat(file_path, "Incorrect encoding ") {
}