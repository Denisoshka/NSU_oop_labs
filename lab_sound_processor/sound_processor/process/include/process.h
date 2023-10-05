#ifndef WAV_PROCESS_H
#define WAV_PROCESS_H

// #include "wav"

#include <array>
#include <boost/program_options.hpp>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

namespace po = boost::program_options;

class process {
public:
  process(size_t sampleRate);
  process(size_t sampleRate, po::variables_map &vm);
  void setSettings(po::variables_map &vm);
  void executeConversions();

private:
  const uint32_t sampleRate_;

  std::string SettingsFile_;
  std::string FileOutPath_;
  std::vector<std::string> FileLinks_;

  std::ifstream SettingsIn_;
  std::ofstream FileOut_;
};

#endif// WAV_PROCESS_H
