<<<<<<< Updated upstream
//
// Created by denis on 11.09.23.
//

#include "wav_reader.h"
#include "wav_reader.h"
#include "../help_include/wav_types.h"
#include "../help_include/wav_errors.h.h"

#include <string>

WavReader::WavReader(const std::string& FilePath) {
  FileIn_.open(FilePath);
  if (FilePath.find(".wav") == std::string::npos){
    return
  }



  std::array<int, 5>;
};
>>>>>>> Stashed changes
