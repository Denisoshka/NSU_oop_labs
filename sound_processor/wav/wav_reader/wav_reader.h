#ifndef SOUND_PROCESSOR_WAV_READER_H
#define SOUND_PROCESSOR_WAV_READER_H

#include "../wav_tmp/wav_tmp.h"

<<<<<<< Updated upstream
class wav_reader :public wav{};
=======
#include <iostream>
#include <fstream>

class WavReader {
public:
  WavReader() = default;
  explicit WavReader(const std::string& FilePath);
  void ReadHeader(std::string FileName){};


private:
  std::ofstream FileIn_;
};
>>>>>>> Stashed changes


#endif// SOUND_PROCESSOR_WAV_READER_H
