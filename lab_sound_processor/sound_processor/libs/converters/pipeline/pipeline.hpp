/*
#pragma once

#include "converter.hpp"

#include <string>
#include <fstream>
#include <queue>

namespace conv {
  class ConverterPipeline {
  public:
    ConverterPipeline();

    bool setTask();
    bool taskFinished();
    void setSettings(std::string&& SettingsPath, std::vector<std::string>&& FileInLinks);
    void executeTask(std::vector<int16_t>& sampleOut, std::vector<int16_t>& samples);

    std::string curFile(const size_t stream) const;

    size_t curReadSecond() const;
    size_t curWriteSecond() const;
    size_t curReadStream() const;

  private:

    void fillPipeline_();
  };
}// namespace conv
*/
