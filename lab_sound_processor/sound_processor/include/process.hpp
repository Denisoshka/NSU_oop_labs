#pragma once

#include "cl_parser.hpp"
#include "converters.hpp"

#include <boost/program_options.hpp>
#include <string>
#include <queue>
#include <fstream>

struct TaskInf {
  std::string converter;
  std::vector<size_t> params;
};



class process {
public:
  process() = default;
  process(boost::program_options::variables_map& vm);
  void setSettings(boost::program_options::variables_map& vm);
  void fillPipeline();
  void executeConversions();

private:
  size_t OutDuration_ = 0;
  int32_t SampleRate_;

  std::queue<TaskInf> Pipeline_;

  std::vector<std::string> FileInPath_;
  std::string SettingsPath_;
  std::string FileOutPath_;

  std::ifstream SettingsStream_;
};

