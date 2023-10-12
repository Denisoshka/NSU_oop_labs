#include "wav.hpp"
#include "wav_exceptions.hpp"

void WAV::makeWAVFile(const std::string& FilePath) {
  if( FilePath.find(".wav") == std::string::npos ) {
    throw IncorrectExtension(FilePath);
  }
  std::ofstream stream{FilePath};
}