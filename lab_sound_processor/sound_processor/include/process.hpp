#ifndef WAV_PROCESS_H
#define WAV_PROCESS_H

#include "cl_parser.hpp"

#include <array>
#include <boost/program_options.hpp>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

namespace po = boost::program_options;

class process {
public:
  process() = default;
  process(const po::variables_map& vm);
  void setSettings(po::variables_map& vm);
  void executeConversions();

private:
  size_t outDuration_ = 0;
  int32_t sampleRate_;
  std::string SettingsFile_;
  std::string FileOutPath_;
  std::vector<std::string> FileLinks_;
};

#endif// WAV_PROCESS_H
