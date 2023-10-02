#ifndef SOUND_PROCESSOR_WAV_READER_H
#define SOUND_PROCESSOR_WAV_READER_H

#include "../wav_tmp/wav_tmp.h"

#include <fstream>
#include <iostream>

class WavReader {
public:
  WavReader() = default;
  explicit WavReader(std::string file_path);
  void open(std::string filePath);

private:
  std::string FilePath_;
  std::ifstream Fin_;
};


#endif//
