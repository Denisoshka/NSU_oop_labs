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
    virtual void setParams(conv::TaskParams& params) = 0;
    virtual size_t getReadSecond() = 0;
    virtual size_t getWriteSecond() = 0;
    [[nodiscard]] virtual bool taskFinished() = 0;
    virtual size_t getReadStream() = 0;
    //    virtual size_t getWriteStream() = 0;

  private:
  };

  class MixConverter: public Converter {
  public:
    MixConverter() = default;
    ~MixConverter() override = default;
    void process(std::vector<int16_t>& mainSample, std::vector<int16_t>& subSample) override;
    size_t getReadSecond() override;
    size_t getWriteSecond() override;
    void setParams(conv::TaskParams& params) override;
    bool taskFinished() override;
    size_t getReadStream() override;
    //    size_t getWriteStream() override;

  private:
    TaskParams taskInf_;
  };

  class MuteConverter: public Converter {
  public:
    MuteConverter() = default;
    ~MuteConverter() override = default;
    void process(std::vector<int16_t>& mainSample, std::vector<int16_t>& subSample) override;
    size_t getReadSecond() override;
    size_t getWriteSecond() override;
    void setParams(TaskParams& params) override;
    bool taskFinished() override;
    size_t getReadStream() override;
    //    size_t getWriteStream() override;

  private:
    TaskParams taskInf_;
  };

  class LowPassConverter: public Converter {
  public:
    LowPassConverter();
    ~LowPassConverter() override = default;
    void process(std::vector<int16_t>& mainSample, std::vector<int16_t>& subSample) override;
    size_t getReadSecond() override;
    size_t getWriteSecond() override;
    void setParams(TaskParams& params) override;
    bool taskFinished() override;
    size_t getReadStream() override;
    //    size_t getWriteStream() override;
    
  private:
    void initLowPassFilter(size_t cutoffFrequency, size_t sampleRate = 44100);
    int16_t applyLowPassFilter(int16_t sample);
    TaskParams taskInf_;
    std::vector<double> coefficients;
    std::vector<double> buffer;
    size_t frequency;
    int num_taps;
  };

  class BassBoostConverter: public Converter {
  public:
    BassBoostConverter() = default;
    ~BassBoostConverter() override = default;
    void process(std::vector<int16_t>& mainSample, std::vector<int16_t>& subSample) override;
    size_t getReadSecond() override;
    size_t getWriteSecond() override;
    void setParams(TaskParams& params) override;
    bool taskFinished() override;
    size_t getReadStream() override;

  private:
    TaskParams taskInf_;
    size_t BassBoostCoeficent_ = 2;
    int16_t bassFactor_ = 1000;

  };

  class CopyConverter: public Converter {
  public:
    CopyConverter() = default;
    ~CopyConverter() override = default;
    void process(std::vector<int16_t>& mainSample, std::vector<int16_t>& subSample) override;
    size_t getReadSecond() override;
    size_t getWriteSecond() override;
    void setParams(conv::TaskParams& params) override;
    bool taskFinished() override;
    size_t getReadStream() override;
    //    size_t getWriteStream() override;

  private:
    TaskParams taskInf_;
    size_t acceleration_ = 1;
  };

  class ConverterPipeline {
  private:
    const int TasksCount_;
    std::map<std::string, std::shared_ptr<Converter>> converters_;

    std::shared_ptr<Converter> curTask_;

    std::regex ConverterName_;
    std::regex StreamName_;
    std::regex Time_;
    std::regex Pass_;

    std::queue<TaskInf> Pipeline_;
    size_t curStream_ = 0;

    std::vector<std::string> FileLinks_;

    std::string SettingsPath_;
    std::ifstream SettingsStream_;

    void fillPipeline_();

  public:
    ConverterPipeline();

    bool setTask();
    bool taskFinished();
    void setSettings(const std::string& SettingsPath, const std::vector<std::string>& FileInLinks);
    void executeTask(std::vector<int16_t>& sampleOut, std::vector<int16_t>& samples);

    std::string curFile(const size_t stream) const;

    //    size_t curStream() const;
    size_t curReadSecond() const;
    size_t curWriteSecond() const;
    size_t curReadStream() const;
    //    size_t curWriteStream() const;
  };
}// namespace conv

#endif// CONVERTERS_CONVERTERS_HPP
