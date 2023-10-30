#pragma once

#include "string"
#include "game_screen.hpp"
#include "game_obj.hpp"

#include "ncurses.h"
#include <vector>

namespace gameProcess {

  class gameProcess {
  public:
    gameProcess(std::string&& gameSettings);
//    void loadGameSettings();

    static int AsyncWGETCH(WINDOW *win);
    int process();

    void initGameProcessEnvironment();
    void initGameScreen(std::vector<std::pair<std::pair<int, int>, std::string>>&& stats);

    void updateGameEnvironment();
    void updatePlayer(const int action);

  protected:
    struct screenSize {
      int height, width, startX0, startY0;
    }mapSize_{}, statsSize_{};
    std::vector<char> gameMap_;
    gameScreen::gameScreen screen_{};

    gameObj::Player player{};
    std::vector<std::shared_ptr<gameObj::ShiftingObject>> gameObjects{};
    std::vector<std::shared_ptr<gameObj::ShiftingObject>> myWeapons{};
    std::vector<std::shared_ptr<gameObj::ShiftingObject>> enemyWeapons{};
  };


class worldProcess : public gameProcess{
  public:
  private:
};

}// namespace gameProcess
