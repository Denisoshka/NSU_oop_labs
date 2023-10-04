#ifndef WAV_CONVERTERS_H
#define WAV_CONVERTERS_H

#include <fstream>
#include <map>
#include <memory>
#include <queue>
#include <string>
#include <vector>

namespace conv {
  enum ConverterNums {
    noConverter,
    mix,
    mute,
  };

  std::map<std::string, ConverterNums> converterIndex{
          {"mix",  mix },
          {"mute", mute}
  };

  struct TaskInf_ {
    ConverterNums converter = noConverter;
    size_t thread = 0;
    std::vector<size_t> params{};
  };

  struct sampleBuffer {
    uint16_t *sample_;
    size_t curLen_;
    size_t len_;
  };

  class Converter {
  public:
    Converter() = default;

    virtual void process(sampleBuffer &sample1, std::vector<sampleBuffer &> &samples,
                         std::vector<size_t> &params) {
    }

  private:
    //  uint16_t *sampleBuffer_;
    //  size_t sampleBufferLen_;
    //  size_t start_;
    //  size_t end_;
  };

  class MuteConverter: Converter {
  public:
    MuteConverter() = default;
    void process(sampleBuffer &sample1, std::vector<sampleBuffer &> &samples,
                 std::vector<size_t> &params) override;

  private:
  };

  class MixConverter: public Converter {
  public:
    MixConverter() = default;
    //  MixConverter(sampleBuffer sample1, sampleBuffer sample2, std::vector<size_t> params);
    void process(sampleBuffer &AddSample, std::vector<sampleBuffer &> &samples,
                 std::vector<size_t> &params) override;

  private:
  };

  class ConverterInterface {
  public:
    //  std::string getTask(size_t &FileIn, size_t &start, size_t &end);
    ConverterInterface() = default;
    void open(std::string &&SettingsFile);
    bool getTask();
    void filTaskList();
    void executeTask(sampleBuffer &sampleOut, std::vector<sampleBuffer &> &samples);

    bool taskFinished() const;

    std::string getCurFile();
    size_t getCurSec() const;

  private:
    const int TasksCount_ = 10;
    bool taskFinished_ = false;

    struct task {
      Converter converter;
      std::vector<size_t> params;
    } task_;

    struct {
      size_t CurFile_;
      size_t CurSecond_;
    } fileInf_;

    std::queue<TaskInf_> taskList_;
    std::vector<std::string> FileLinks_;
    std::string SettingsPath_;
    std::ifstream SettingsStream_;
  };
}// namespace conv
#endif// WAV_CONVERTERS_H
