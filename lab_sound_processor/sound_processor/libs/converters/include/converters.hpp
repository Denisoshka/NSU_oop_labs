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
    copy,
    bass,
  };

  enum settingsPos {
    converter,
    stream,
    timeStart,
    timeEnd,
  };

  struct TaskParams {
    std::vector<size_t> otherParams{};
    size_t stream = 0;
    size_t startTime = 0;
    size_t curSec = 0;
    size_t endTime = SIZE_MAX;
    bool taskFinished = false;
  };

  struct TaskInf {
    std::shared_ptr<Converter> converter;
    TaskParams params;
  };

  class Converter {
  public:
    Converter() = default;
    virtual ~Converter() = default;
    virtual void process(std::vector<int16_t>& mainSample, std::vector<int16_t>& subSample) = 0;
    virtual void setParams(conv::TaskParams& params);
    virtual size_t getReadSecond();
    virtual size_t getWriteSecond();
    [[nodiscard]] bool taskFinished() const;

  private:
    TaskParams taskInf_;
  };

  class MixConverter: public Converter {
  public:
    MixConverter() = default;
    ~MixConverter() override = default;
    void process(std::vector<int16_t>& mainSample, std::vector<int16_t>& subSample) override;
    void setParams(conv::TaskParams& params) override;
    //    size_t getReadSecond() ;
    //    size_t getWriteSecond();

  private:
    TaskParams taskInf_;
  };

  class MuteConverter: public Converter {
  public:
    MuteConverter() = default;
    ~MuteConverter() override = default;
    void process(std::vector<int16_t>& mainSample, std::vector<int16_t>& subSample) override;
    //    void setParams(TaskParams&& params) override;
    //    size_t getReadSecond() override;
    //    size_t getWriteSecond() override;

  private:
    TaskParams taskInf_;
  };

  class BassBoostConverter: public Converter {
  public:
    BassBoostConverter() = default;
    ~BassBoostConverter() override = default;
    void process(std::vector<int16_t>& mainSample, std::vector<int16_t>& subSample) override;
//        void setParams(TaskParams&& params) ;
    //    size_t getReadSecond() override;
    //    size_t getWriteSecond() override;

  private:
    TaskParams taskInf_;
  };

  class CopyConverter: public Converter {
  public:
    CopyConverter() = default;
    ~CopyConverter() override = default;
    void process(std::vector<int16_t>& mainSample, std::vector<int16_t>& subSample) override;
    void setParams(conv::TaskParams& params) override;
    //    size_t getReadSecond() override;
    size_t getWriteSecond() override;

  private:
    TaskParams taskInf_;
    size_t acceleration_;
  };

  class ConverterInterface {
  private:
    const int TasksCount_ = 10;
    std::map<std::string, std::shared_ptr<Converter>> converters_;

    //{"bass", std::make_shared<BassBoostConverter>()},
    //{"copy", std::make_shared<CopyConverter>()     },

    std::shared_ptr<Converter> curTask_;

    std::regex ConverterName_ = std::regex(R"(\w+)");
    std::regex StreamName_ = std::regex(R"(\$\d+)");
    std::regex Time_ = std::regex(R"(\d+)");
    std::regex Pass_ = std::regex(R"(--)");

    std::queue<TaskInf> Pipeline_;
    size_t curStream_ = 0;

    std::vector<std::string> FileLinks_;

    std::string SettingsPath_;
    std::ifstream SettingsStream_;

    void fillPipeline_();

  public:
    ConverterInterface();

    bool setTask();
    bool taskFinished();

    void setSettings(const std::string& SettingsPath, const std::vector<std::string>& FileInLinks);
    void executeTask(std::vector<int16_t>& sampleOut, std::vector<int16_t>& samples);

    std::string curFile(const size_t stream) const;

    size_t curStream() const;
    size_t curReadSecond() const;
    size_t curWriteSecond() const;
  };
}// namespace conv

#endif// CONVERTERS_CONVERTERS_HPP
