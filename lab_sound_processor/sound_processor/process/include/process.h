#ifndef WAV_PROCESS_H
#define WAV_PROCESS_H

// #include "wav"

#include <fstream>
#include <string>
#include <vector>



class process {
public:
  bool getTasks();
  void startConversion(std::string &&FilePath);

private:
  //  std::vector
  std::string SettingsFile_;
  std::string FilePath_;
  std::ofstream FileOut_;
};

#endif// WAV_PROCESS_H
