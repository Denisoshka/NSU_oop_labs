#pragma once

#include <boost/program_options.hpp>
#include <fstream>
#include <queue>
#include <string>

namespace process {
  struct TaskInf {
    std::string Converter;
    std::vector<size_t > InStreams;
    std::vector<size_t> Params;
  };

  class Pipeline {
  public:
    Pipeline(const std::string& kSettingsPath, const size_t kTasksCount);
    bool empty() const;
    TaskInf pop();
    void fill();

  private:
    size_t TasksCount_;
    std::string SettingsPath_;
    std::ifstream SettingsStream_;
    std::queue<TaskInf> Container_{};
  };

  class Process {
  public:
    Process() = default;
    Process(const boost::program_options::variables_map& kVM);
    void setSettings(const boost::program_options::variables_map& kVM);
    void executeConversions();

  private:
    size_t OutDuration_ = 0;
    int32_t SampleRate_;

    std::vector<std::string> FileInPath_;
    std::string SettingsPath_;
    std::string FileOutPath_;

    std::ifstream SettingsStream_;
  };

  void printConverterDesc(const std::string& kProgramName, const std::string& kUsage,
                          const std::string& kDescription);
}// namespace process
