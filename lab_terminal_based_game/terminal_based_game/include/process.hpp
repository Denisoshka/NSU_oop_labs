#pragma once

#include <vector>
#include "ncurses.h"
#include "string"

namespace gameProcess {

  class gameProcess {
  public:
    gameProcess(std::string&& gameSettings);
//    void loadGameSettings();

    static int AsyncWGETCH(WINDOW *win);
    int process();

  private:
    struct screenSize {
      int height, width, startX0, startY0;
    }mapSize_, statsSize_;
    std::vector<char> gameMap_;
  };

}// namespace gameProcess
