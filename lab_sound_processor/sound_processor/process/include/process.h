#ifndef WAV_PROCESS_H
#define WAV_PROCESS_H

// #include "wav"

#include <array>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

class process {
public:
  process(size_t sampleRate);

  void executeConversions(std::string &&FilePath);

private:
  const uint32_t sampleRate_;
  std::unique_ptr<uint16_t[]> sample_;
  std::string SettingsFile_;
  std::string FilePath_;
  std::ifstream SettingsIn_;
  std::ofstream FileOut_;
};

#endif// WAV_PROCESS_H
