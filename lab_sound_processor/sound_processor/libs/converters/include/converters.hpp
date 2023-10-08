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

  enum settingsPos{
    converter,
    stream,
    timeStart,
    timeEnd,
  };

  struct sampleBuffer {
    uint16_t *sample_;
    size_t curLen_;
    size_t len_;
  };

  class Converter {
  public:
    Converter() = default;
    virtual ~Converter() = default;
    virtual void process(sampleBuffer &sample1, std::vector<sampleBuffer> &samples,
                         std::vector<size_t> &params) = 0;

  private:
  };

  class MixConverter: public Converter {
  public:
    MixConverter() = default;
    ~MixConverter() override= default;
    void process(sampleBuffer &sample1, std::vector<sampleBuffer> &samples,
                 std::vector<size_t> &params) override;

  private:
  };

  class MuteConverter: public conv::Converter {
  public:
    MuteConverter() = default;
    ~MuteConverter() override = default;
    void process(conv::sampleBuffer &sample1, std::vector<conv::sampleBuffer> &samples,
                 std::vector<size_t> &params) override;
  private:
  };
  class MuteConverter;
  class MixConverter;

  class ConverterInterface {
  public:
    //  std::string getTask(size_t &FileIn, size_t &start, size_t &end);
    ConverterInterface() = default;
    //    void open(std::string &&SettingsFile);
    bool setTask();
    void setSettings(std::string &FilePath, std::vector<std::string> &fileLinks);
    void setFileLinks(const std::vector<std::string> &fileLinks);
    void fillPipeline();
    void executeTask(sampleBuffer &sampleOut, std::vector<sampleBuffer> &samples);

    bool taskFinished() const;

    std::string curFile();
    size_t curSec() const;

  private:
    void setFileLinks(std::vector<std::string> &fileLinks);

    const int TasksCount_ = 10;
    bool TaskFinished_ = false;

    struct TaskInf_ {
      std::shared_ptr<Converter> converter = nullptr;
      size_t stream = 0;
      bool taskFinished = false;
      size_t startTime = 0;
      size_t endTime = SIZE_MAX;
      std::vector<size_t> params{};
    } curTask_;

    /*
    struct task {
      Converter converter;
      std::vector<size_t> params;
    } Task_;*/

    struct {
      size_t CurFile_ = 0;
      size_t CurSecond_ = 0;
    } FileInf_;

    std::map<std::string, std::shared_ptr<Converter>> converters_{
            {"mix",  std::make_shared<MixConverter>() },
            {"mute", std::make_shared<MuteConverter>()}
    };

    std::regex ConverterName_ = std::regex(R"(\w+)");
    std::regex StreamName_ = std::regex(R"($\d+)");
    std::regex Time_ = std::regex(R"(\d+)");
    std::regex Pass_ = std::regex(R"(--)");

    std::queue<TaskInf_> Pipeline_;

    std::vector<std::string> FileLinks_;
    std::string SettingsPath_;

    std::ifstream SettingsStream_;
  };
}// namespace conv

#endif// CONVERTERS_CONVERTERS_HPP
