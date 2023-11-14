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

  class GameController {
  public:
    GameController(gScreen::gameScreen& screen);
    void updateGameContext(const int kAction);
    void checkCollisions();
    void drawGameContext(const int kAction);
  protected:
    gScreen::gameScreen& gscreen_;
    gameObj::Player player{};
    std::vector<std::shared_ptr<gameObj::ShiftingObject>> gameObjects_{};
    std::vector<std::shared_ptr<gameObj::ShiftingObject>> Trace_;
  };

  class gameProcess {
  public:
    gameProcess() = default;
    int process();

  protected:
    struct shootMode {};

    enum gameMode {
      ekRateMode,
      ekTrainingMode,
    };

    gScreen::CoreScreen coreScreen;
    gScreen::BasicScreen mainScreen_{coreScreen};


    int showMenu(std::string& playerName);
    void initShootModeEnvironment(gScreen::gameScreen& gscreen, shootMode& environmentInf);

    void updateGameProcess(gScreen::gameScreen& gscreen, shootMode& environmentInf);
    void updatePlayer(gScreen::gameScreen& gscreen, shootMode& environmentInf, const int action);
    void updateMyWeapons(gScreen::gameScreen& gscreen, shootMode& environmentInf);
    void updateEnemyWeapons(gScreen::gameScreen& gscreen, shootMode& environmentInf);
    int startGame(gameMode mode, const std::string& kPlayerName);
    void startRate(gScreen::gameScreen& gscreen, const std::string& kPlayerName);
    void startTraining(gScreen::gameScreen& gscreen);
    void updateScore(const std::string& playerName, const int score);
    auto eraseProcessObject(gScreen::gameScreen& gscreen, auto objectIterator,
                            std::vector<std::shared_ptr<gameObj::ShiftingObject>>& objects);
    void checkMyCollisions(gScreen::gameScreen& gscreen, shootMode& environmentInf);
    void checkEnemyCollisions(gScreen::gameScreen& gscreen, shootMode& environmentInf);
  };

}// namespace gameProcess
