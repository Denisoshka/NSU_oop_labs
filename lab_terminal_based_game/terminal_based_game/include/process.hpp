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

  struct TerminationConditions {
    bool PlayerIsDead = false;
    bool GameIsEnd = false;
  };

  class GameController {
  public:
    GameController(gScreen::gameScreen& screen);

    void updateGameContext(const int kAction);
    void drawGameContext();

    TerminationConditions getTerminationConditions() const;
    bool gameIsEnd() const;

  protected:
    gScreen::gameScreen& gscreen_;
    gScreen::windowSettings mapSize_;

    TerminationConditions conditions_{};

    gameObj::Player player_{};
    std::vector<std::shared_ptr<gameObj::ShiftingObject>> gameObjects_{};
    std::vector<std::shared_ptr<gameObj::ShiftingObject>> Trace_;

    void checkCollisions();
    void updateGameRotations();
    void updateGameConditions(const int kAction);
  };

  class gameProcess {
  public:
    gameProcess() = default;
    int process();

  protected:
    enum gameMode {
      ekRateMode,
      ekTrainingMode,
    };

    gScreen::CoreScreen coreScreen;
    gScreen::BasicScreen mainScreen_{coreScreen};

    int showMenu(std::string& playerName);
    int startGame(gameMode mode, const std::string& kPlayerName);
    void startRate(gScreen::gameScreen& gscreen, const std::string& kPlayerName);
    void updateScore(const std::string& playerName, const int score);
  };

}// namespace gameProcess
