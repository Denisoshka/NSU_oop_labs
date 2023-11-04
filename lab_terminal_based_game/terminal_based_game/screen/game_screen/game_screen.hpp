#pragma once

#include "basic_screen.hpp"

namespace screeen {
  class gameScreen: basicScreen {
  public:
    gameScreen(const basicScreen& screen, const std::vector<int>& gameMapSize,
               const std::vector<int>& gameStatsSize, std::vector<char>&& map,
               char emptySpace,
               std::vector<std::pair<std::pair<int, int>, std::string>>&& stats);
    virtual ~gameScreen();
    void updateGameStat(const std::string& key, std::string&& value);
    void drawMoveGameObj(const std::pair<int, int>& objectCoords,
                         const std::pair<int, int>& objectShift, const char avatar);
    void deleteGameObj(const std::pair<int, int>& objectCoords);
    bool fixCoordsToMove(const std::pair<int, int>& objectCoords, std::pair<int, int>& objectShift);

  protected:
    void loadGameMap(std::vector<char>&& map, char emptySpace);
    void drawGameMap();

    void loadGameStats(std::vector<std::pair<std::pair<int, int>, std::string>>&& stats);
    void drawGameStats();

    std::vector<char> gameMap_;
    char emptySpace_;
    subWindowSize gameMapSize_{}, gameStatsSize_{};
    std::map<std::string, std::pair<int, int>> gameStats_;
  };
}// namespace screeen