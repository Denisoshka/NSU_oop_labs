#include "wav_reader.h"
#include "../wav_errors/wav_errors.h"


WavReader::WavReader(std::string file_path) {
}

void WavReader::open(std::string filePath) {
  FilePath_ = std::move(filePath);
  if ( FilePath_.find(".wav")){
    throw IncorrectExtension(FilePath_);
  }

  Fin_.open(FilePath_, std::ios_base::binary);
  if (Fin_.good()){
    throw FileNotOpen(FilePath_);
  }

  checkHeader();






}


