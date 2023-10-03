#include "wav.h"

void WAVWriter::writeSample(uint16_t *sampleBuffer, size_t sampleBufferLen, size_t start,
                            size_t end) {
  FileOut_.seekp(dataStart_ + start , std::ios_base::beg);
  FileOut_.write(reinterpret_cast<char * >(sampleBuffer), sizeof(*sampleBuffer)* sampleBufferLen);
}
