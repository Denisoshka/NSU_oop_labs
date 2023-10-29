#pragma once

#include <curses.h>
#include <memory>
#include <vector>

namespace gameScreen {
  class gameScreen {
  public:
    //    explicit gameScreen(WINDOW *window, unsigned terminalHeight, unsigned terminalWidth);
    gameScreen(const std::vector<int>& gameMapSize, const std::vector<int>& gameStatsSize);
    virtual ~gameScreen();

    void initScreen();
//    int screenInput();
    WINDOW * getWindow();
    void loadGameMap(std::vector<char>&& map, char wall);
    void drawGameMap();

    void loadGameStats(std::vector<std::pair<std::pair<int, int>, std::string>>&& kStats);
    void drawGameStats();
    void fixGameStats(std::vector<std::pair<std::pair<int, int>, int>>& kStats);

    void drawGameObj(const std::pair<int, int>& objectCoords,
                     const std::pair<int, int>& objectShift, const char avatar);
    bool fixCoordsToMove(const std::pair<int, int>& objectCoords, std::pair<int, int>& objectShift);

  protected:
    struct subWindowSize {
      int width, height, startX = 0, startY = 0;
    } gameMapSize_, gameStatsSize_;

    std::vector<char> map_;
    char emptySpace_;
    WINDOW *window_;
    std::pair<int, int> terminalSize_;
  };
}// namespace gameScreen
