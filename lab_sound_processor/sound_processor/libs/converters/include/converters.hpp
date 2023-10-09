#ifndef CONVERTERS_CONVERTERS_HPP
#define CONVERTERS_CONVERTERS_HPP

#include <boost/tokenizer.hpp>
#include <fstream>
#include <map>
#include <memory>
#include <queue>
#include <regex>
#include <string>
#include <vector>

namespace conv {
  class Converter;

  enum ConverterNums {
    noConverter,
    mix,
    mute,
  };

  enum settingsPos {
    converter,
    stream,
    timeStart,
    timeEnd,
  };

  struct sampleBuffer {
    uint16_t* sample_;
    size_t curLen_;
    size_t len_;
  };

  struct TaskInf {
    std::shared_ptr<Converter> converter = nullptr;
    std::vector<size_t> otherParams{};
    size_t stream = 0;
    size_t startTime = 0;
    size_t curSec = 0;
    size_t endTime = SIZE_MAX;
    bool taskFinished = false;
  };

  class Converter {
  public:
    Converter() = default;
    virtual ~Converter() = default;
    virtual void process(std::vector<int16_t>& mainSample, std::vector<int16_t>& subSample, TaskInf& params) = 0;

  private:
  };

  class MixConverter: public Converter {
  public:
    MixConverter() = default;
    ~MixConverter() override = default;
    void process(std::vector<int16_t>& mainSample, std::vector<int16_t>& subSample,
                 TaskInf& params) override;

  private:
  };

  class MuteConverter: public conv::Converter {
  public:
    MuteConverter() = default;
    ~MuteConverter() override = default;
    void process(std::vector<int16_t>& mainSample, std::vector<int16_t>& subSample,
                 TaskInf& params) override;

  private:
  };

  class ConverterInterface {
  public:
    ConverterInterface() = default;
    bool setTask();
    void setSettings(const std::string& SettingsPath, const std::vector<std::string>& FileInLinks);
    void executeTask(std::vector<int16_t>& sampleOut, std::vector<int16_t>& samples);
    bool taskFinished() const;
    size_t curStream() const;
    std::string curFile(const size_t stream) const;
    size_t curSec() const;

  private:
    void fillPipeline_();
    void setFileLinks_(const std::vector<std::string>& fileLinks);
    //    void setFileLinks(std::vector<std::string> &fileLinks);

    const int TasksCount_ = 10;
    TaskInf curTask_;

    std::map<std::string, std::shared_ptr<Converter>> converters_{
            {"mix",  std::make_shared<MixConverter>() },
            {"mute", std::make_shared<MuteConverter>()}
    };

    std::regex ConverterName_ = std::regex(R"(\w+)");
    std::regex StreamName_ = std::regex(R"(\$\d+)");
    std::regex Time_ = std::regex(R"(\d+)");
    std::regex Pass_ = std::regex(R"(--)");

    std::queue<TaskInf> Pipeline_;

    std::vector<std::string> FileLinks_;

    std::string SettingsPath_;
    std::ifstream SettingsStream_;
  };
}// namespace conv

#endif// CONVERTERS_CONVERTERS_HPP
