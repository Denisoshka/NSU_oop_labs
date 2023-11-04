#pragma once

#include "basic_screen.hpp"

namespace screeen {
  class gameMenu: basicScreen {
  public:
    gameMenu(const basicScreen& screen, const std::vector<int>& gameMenuSize);
    virtual ~gameMenu() = default;

    //    void loadGameMenu(std::vector < std::pair<std::string> stats);
    void drawGameMenu(const std::string& kScorePath);
    void drawGameMenu(const std::vector<std::pair<std::pair<int, int>, std::string>>& stats);
    void updateGameMenu(const std::string_view& key, std::string_view&& value);
    void drawGameMenu(const std::vector<std::pair<std::string, int>>& score);
    void drawScoreTable(const std::vector<std::pair<std::string, int>>& score);
    void drawGameName();
    void drawNameInsertField();

  protected:
    subWindowSize gameScoreSize_{};
    void drawGameMenu(const std::vector<int>& score);
    void drawGameMenu(const std::vector<std::string, int>& score);
  };
}// namespace screeen
