#pragma once

#include <curses.h>
#include <map>
#include <memory>
#include <vector>

namespace gameScreen {
  class defaultScreen {
  public:
    //    explicit defaultScreen(WINDOW *window, unsigned terminalHeight, unsigned terminalWidth);
    defaultScreen();
    defaultScreen(const std::vector<int>& gameMapSize, const std::vector<int>& gameStatsSize);
    defaultScreen& operator=(defaultScreen&& otherScreen);
    virtual ~defaultScreen();

    void loadGameScreenInf(
            const std::vector<int>& gameMapSize, const std::vector<int>& gameStatsSize,
            std::vector<char>&& map,
            std::vector<std::pair<std::pair<int, int>, std::string_view>>&& stats, char emptySpace);


    void initScreen();
    int screenInput();
    WINDOW *getWindow();
    void loadGameMap(std::vector<char>&& map, char wall);
    void drawGameMap();

    void loadGameStats(std::vector<std::pair<std::pair<int, int>, std::string_view>>&& stats);
    void drawGameStats();
    void updateGameStat(const std::string_view& key, std::string_view&& value);

    void initGameMenu();

    void loadGameMenu(std::vector<std::pair<std::pair<int, int>, std::string_view>>&& stats);
    void drawGameMenu(const std::string& kStatsPath);
    void drawGameMenu(const std::vector<std::pair<std::pair<int, int>, std::string>>& stats);
    void updateGameMenu(const std::string_view& key, std::string_view&& value);


    void drawMoveGameObj(const std::pair<int, int>& objectCoords,
                         const std::pair<int, int>& objectShift, const char avatar);
    void deleteGameObj(const std::pair<int, int>& objectCoords);
    bool fixCoordsToMove(const std::pair<int, int>& objectCoords, std::pair<int, int>& objectShift);

  protected:
    struct subWindowSize {
      int width, height, startX = 0, startY = 0;
    } gameMapSize_{}, gameStatsSize_{}, terminalSize_{}, menuSize_{};

    //    std::vector<std::pair<std::pair<int, int>, std::string>>&& gameStats_;
    std::map<std::string_view, std::pair<int, int>> gameStats_;
    std::vector<char> gameMap_;
    char emptySpace_;
    WINDOW *window_;
    //    std::pair<int, int> terminalSize_;

  };


}// namespace gameScreen
