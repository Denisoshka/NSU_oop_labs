#ifndef WAV_WAV_WRITER_H
#define WAV_WAV_WRITER_H

#include "../wav_tmp/wav_tmp.h"

#include <iostream>

class WAVwriter {
public:
  WAVwriter() = default;
  explicit WAVwriter(std::string& FilePath){};
  void writeSample(){};

private:
  std::ofstream OutFile_;
};


#endif// WAV_WAV_WRITER_H
