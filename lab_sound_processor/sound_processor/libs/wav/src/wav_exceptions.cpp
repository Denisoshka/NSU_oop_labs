#include "wav_exceptions.hpp"

IncorrectFileFormat::IncorrectFileFormat(const std::string& filePath, const std::string& desc)
    : std::invalid_argument(filePath + ": Incorrect file format : " + desc) {
}

IncorrectExtension::IncorrectExtension(const std::string& filePath)
    : IncorrectFileFormat(filePath, "Incorrect file extension") {
}

IncorrectRIFFHeader::IncorrectRIFFHeader(const std::string& filePath)
    : IncorrectFileFormat(filePath, "Incorrect RIFF header") {
}

IncorrectFormatType::IncorrectFormatType(const std::string& filePath)
    : IncorrectFileFormat(filePath, "Incorrect format type") {
}

IncorrectFormatData::IncorrectFormatData(const std::string& filePath)
    : IncorrectFileFormat(filePath, "Reading format data error") {
}

IncorrectAudioFormat::IncorrectAudioFormat(const std::string& filePath)
    : IncorrectFileFormat(filePath, "Incorrect audio format") {
}

IncorrectChannelsNumber::IncorrectChannelsNumber(const std::string& filePath)
    : IncorrectFileFormat(filePath, "Incorrect channels number") {
}

IncorrectBitsPerSample::IncorrectBitsPerSample(const std::string& filePath)
    : IncorrectFileFormat(filePath, "Incorrect bits quantity per sample") {
}

IncorrectSampleRate::IncorrectSampleRate(const std::string& filePath)
    : IncorrectFileFormat(filePath, "Incorrect sample rate") {
}

ChunkNotFound::ChunkNotFound(const std::string& filePath, uint32_t chunk_ID)
    : IncorrectFileFormat(filePath, "Searching chunk " + std::to_string(chunk_ID) + " error") {
}

StreamFailure::StreamFailure(const std::string& filePath)
    : std::ios_base::failure(filePath + " failure ") {
}

IncorrectEncodingFormat::IncorrectEncodingFormat(const std::string& file_path)
    : IncorrectFileFormat(file_path, "Incorrect encoding ") {
}