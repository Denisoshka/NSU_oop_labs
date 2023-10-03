#ifndef PROCESS_CONVERTERS_H
#define PROCESS_CONVERTERS_H

#include <fstream>
#include <memory>
#include <queue>
#include <string>
#include <vector>

class Converter {
public:
  Converter() = default;
private:
  uint16_t * sampleBuffer_;
  size_t sampleBufferLen_;
  size_t start_;
  size_t end_;
};

class MuteConverter: Converter {
public:
  MuteConverter(uint16_t *sampleBuffer, size_t sampleBufferLen, size_t start, size_t end,
                size_t fileSIze);
  void convert(uint16_t *sampleBuffer, size_t sampleBufferLen, size_t start, size_t end,
               size_t fileStart, size_t fileEnd);
private:
//  uint16_t sampleBuffer_;
//  size_t sampleBufferLen_;
//  size_t start_;
//  size_t end_;
};

class MixConverter: public Converter {
public:
private:
};

class ConverterInterface {
public:
  std::string getTask(size_t &FileIn,size_t &start, size_t &end );
  void filTaskList();
  void executeTask(uint16_t *sampleBuffer, size_t sampleBufferLen);
  bool taskFinished();

private:
  std::ifstream SettingsFile_;
  std::queue<std::string> taskList_;
  std::string task_;
  bool taskFinisfed = false;
  size_t totalBytes;
  size_t changedBytes;
};

#endif// PROCESS_CONVERTERS_H
