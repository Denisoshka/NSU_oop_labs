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

  class BasicGameController {
  public:
    BasicGameController(const gScreen::BasicScreen& kScreen, const std::string& kGameScreenSettings,
                        const std::string& kGameMapSettings);
    virtual void initGameContext() = 0;
    virtual void updateGameContext(const int kAction) = 0;
    virtual void drawGameContext() = 0;
    virtual bool gameIsEnd() const = 0;
    virtual int input() = 0;

  protected:
    std::string gameMap_{};
    gScreen::windowSettings gameMapSize_{};
    gScreen::GameScreen gscreen_;
    char emptySpace_;

    void loadGameMap(const std::string& kGameMapSettings);
  };

  class LolGameController: BasicGameController {
  public:
    LolGameController(const gScreen::BasicScreen& kScreen, const std::string& kGameScreenSettings,
                      const std::string kGameMapSettings);
    virtual void initGameContext() override;
    virtual void updateGameContext(const int kAction) override;
    virtual void drawGameContext() override;
    virtual void updateStat(const std::string& kField, const int kValue);
    virtual int input() override;

    std::vector<std::pair<bool, bool>> fixCollision(const std::vector<std::pair<int, int>>& Route);

    TerminationConditions getTerminationConditions() const;

    virtual bool gameIsEnd() const override;

  protected:
    TerminationConditions conditions_{};

    std::shared_ptr<gameObj::Player> player_{};
    std::vector<std::shared_ptr<gameObj::ShiftingObject>> gameObjects_{};
    std::vector<std::shared_ptr<gameObj::ShiftingObject>> Trace_;

    bool isEmptySpase(const int x);

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

    gScreen::CoreScreen coreScreen_;
    gScreen::BasicScreen mainScreen_{coreScreen_};

    int showMenu(std::string& playerName);
    int startGame(gameMode mode, const std::string& kPlayerName);
    void startRate(const std::string& kPlayerName);
    void updateScore(const std::string& playerName, const int score);
  };

}// namespace gameProcess
