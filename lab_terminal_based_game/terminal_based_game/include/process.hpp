#pragma once

#include "game_obj.hpp"
#include "game_screen.hpp"
#include "string"

#include <vector>
#include "ncurses.h"

namespace gameProcess {
  enum gameProcessConstants {
    ekNothingHappenedInTiananmenSquare = 0,
    ekPlayerDead,
  };

  class gameProcess {
  public:
    gameProcess(std::string&& gameSettings);
    //    void loadGameSettings();

    static int AsyncWGETCH(WINDOW *win);
    int process();


  protected:
    std::vector<std::pair<std::string, int>> readGameStats();
    void showMenu();
    void saveGameStats();
    void loadGameMap(std::vector<char>& map);

    void initGameProcessEnvironment();
    void initGameScreen(std::vector<std::pair<std::pair<int, int>, std::string_view>>&& stats);
    std::vector<std::pair<std::pair<int, int>, std::string>> fillGameMenu(
            std::vector<std::pair<std::pair<int, int>, std::string>>& score,
            const std::string& kScorePath);

    void updateGameEnvironment();
    void updateGameProcess();
    void updatePlayer(const int action);
    void updateMyWeapons();
    gameProcessConstants updateEnemyWeapons();

    struct screenSize {
      int height, width, startX0, startY0;
    } screenSize_{}, mapSize_{}, terminalSize_{};

    int playerLives = 5;
    gameScreen::defaultScreen mainScreen_{};
//    gameScreen::gameScreen screen_{};
    std::string gameSettings_;
    gameObj::Player player{};
    std::vector<std::shared_ptr<gameObj::ShiftingObject>> gameObjects{};
    std::vector<std::shared_ptr<gameObj::ShiftingObject>> myWeapons{};
    std::vector<std::shared_ptr<gameObj::ShiftingObject>> enemyWeapons{};
    int startGame();
  };

  class worldProcess: public gameProcess {
  public:
  private:
  };

}// namespace gameProcess
