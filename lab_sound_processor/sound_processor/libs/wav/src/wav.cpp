#include "wav.hpp"

WAV::SampleBuffer::SampleBuffer(size_t sampleRate)
    : sample_(std::make_unique<uint16_t[]>(sampleRate))
    , curLen_(sampleRate)
    , len_(sampleRate) {
}

uint16_t* WAV::SampleBuffer::get() {
  return sample_.get();
}

size_t WAV::SampleBuffer::size() const {
  return len_;
}
