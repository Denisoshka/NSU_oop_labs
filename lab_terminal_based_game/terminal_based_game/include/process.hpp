#pragma once

#include "game_obj.hpp"
#include "screen.hpp"
#include "string"

#include <vector>
#include "ncurses.h"

namespace gameProcess {
  enum gameProcessConstants {
    ekNothingHappened = 0,
    ekPlayerDead,
  };

  class gameProcess {
  public:
    gameProcess() = default;
    int process();

  protected:
    struct shootMode {
      int playerLives;
      gameObj::Player player{};
      std::vector<std::shared_ptr<gameObj::ShiftingObject>> gameObjects{};
      std::vector<std::shared_ptr<gameObj::ShiftingObject>> myWeapons{};
      std::vector<std::shared_ptr<gameObj::ShiftingObject>> enemyWeapons{};
    };

    enum gameMode{
      ekRateMode,
      ekTrainingMode,
    };

    gScreen::coreScreen coreScreen;
    gScreen::basicScreen mainScreen_{coreScreen};


    int showMenu(std::string& playerName);
    void initShootModeEnvironment(gScreen::gameScreen& gscreen, shootMode& environmentInf);

    void updateGameProcess(gScreen::gameScreen& gscreen, shootMode& environmentInf);
    void updatePlayer(gScreen::gameScreen& gscreen, shootMode& environmentInf, const int action);
    void updateMyWeapons(gScreen::gameScreen& gscreen, shootMode& environmentInf);
    gameProcessConstants updateEnemyWeapons(gScreen::gameScreen& gscreen,shootMode& environmentInf);
    int startGame(gameMode mode, const std::string& kPlayerName);
    void startRate(gScreen::gameScreen& gscreen, const std::string& kPlayerName);
    void startTraining(gScreen::gameScreen& gscreen);
    void updateScore(const std::string & playerName, const int score);
    auto eraseGameObject(gScreen::gameScreen& gscreen, auto weapon, shootMode& environmentInf);
  };

}// namespace gameProcess
