#include "process.hpp"
#include "game_obj.hpp"
#include "screen.hpp"
#include "shifting_object.hpp"

#include <chrono>
#include <fstream>
#include <future>
#include <iostream>
#include <random>
#include <set>

namespace {
  const int gkMaxFieldsQuantity = 30;
  const int gkMinimalAmmoQuantity = 10;
  const char gkQuit{']'};
  const std::string gkBulletsField = "bullets quantity: ";
  const std::string gkElapsedTimeField = "elapsed time: ";
  const std::string gkPlayerLives = "lives: ";
  const int gkEnemyQuantity = 0;

  const std::string gkDefPlayerName = "unknown player";

  const std::string gkBasicGameMenuPath = "game_menu.json";
  const std::string gkBasicGameScreenPath = "game_screen.json";
  const std::string gkBasicScorePath = "score.json";

  const char *gkScoreTitle = "score";
  const char *gkScorePlayer = "player";
  const char *gkScoreScore = "score";
  const char *gkScoreMaxFieldsQuantity = "max_quantity";
  const char *gkScoreFields = "fields";
}// namespace

namespace gameProcess {
  int gameProcess::showMenu(std::string& playerName) {
    gScreen::gameMenu menu{mainScreen_, gkBasicGameMenuPath, gkBasicScorePath};
    int input;
    while( (input = menu.screenInput()) != gkQuit ) {
      if( menu.introducePlayerName(input) ) {
        break;
      }
    }
    playerName = menu.getPlayerName();
    return input;
  }

  int gameProcess::process() {
    while( mainScreen_.screenInput() != gkQuit ) {
      std::string playerName{};

      if( showMenu(playerName) == gkQuit ) {
        break;
      }
      playerName = (playerName.empty()) ? gkDefPlayerName : playerName;
      startGame(gameMode::ekRateMode, playerName);
    }

    return 0;
  }

  void gameProcess::startRate(gScreen::gameScreen& gscreen, const std::string& kPlayerName) {
    srandom(time(nullptr));
    auto startGameTime = std::chrono::steady_clock::now();
    int input;

    GameController controller{gscreen};

    while( (input = gscreen.screenInput()) != gkQuit && !controller.gameIsEnd() ) {

      controller.updateGameContext(input);
      controller.drawGameContext();

      auto end = std::chrono::steady_clock::now();
      auto seconds = std::chrono::duration_cast<std::chrono::seconds>(end - startGameTime).count();
      gscreen.updateGameStat(gkElapsedTimeField, seconds);
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    if( controller.getTerminationConditions().GameIsEnd ) {
      auto gameEndTime = std::chrono::steady_clock::now();
      const int elapsedTime =
              std::chrono::duration_cast<std::chrono::seconds>(gameEndTime - startGameTime).count();
      updateScore(kPlayerName, elapsedTime);
    }
  }

  int gameProcess::startGame(gameMode mode, const std::string& kPlayerName) {
    gScreen::gameScreen gscreen{mainScreen_, gkBasicGameScreenPath};
    if( mode == gameMode::ekRateMode ) {
      startRate(gscreen, kPlayerName);
    }

    return 0;
  }

  static void scanScore(std::vector<std::pair<std::string, int>>& arr) {
    boost::property_tree::ptree scores;
    boost::property_tree::read_json(gkBasicScorePath, scores);

    for( auto row = scores.get_child(gkScoreTitle).get_child(gkScoreFields).begin();
         row != scores.get_child(gkScoreTitle).get_child(gkScoreFields).end(); ++row ) {
      arr.emplace_back(row->second.get<std::string>(gkScorePlayer),
                       row->second.get<int>(gkScoreScore));
    }

    std::sort(arr.begin(), arr.end(),
              [](auto& left, auto& right) { return left.second < right.second; });

    arr.resize(std::min<size_t>(gkMaxFieldsQuantity, arr.size()));
  }

  static void writeScore(const std::vector<std::pair<std::string, int>>& arr,
                         boost::property_tree::ptree& json) {
    boost::property_tree::ptree newScore;
    boost::property_tree::ptree jsonScoreFields;

    for( const auto& values: arr ) {
      boost::property_tree::ptree player;
      player.put(gkScorePlayer, values.first);
      player.put(gkScoreScore, values.second);
      jsonScoreFields.push_back({"", player});
    }

    newScore.put(gkScoreMaxFieldsQuantity, gkMaxFieldsQuantity);
    newScore.add_child(gkScoreFields, jsonScoreFields);
    json.add_child(gkScoreScore, newScore);
  }

  void gameProcess::updateScore(const std::string& playerName, const int score) {
    std::vector<std::pair<std::string, int>> arr{};
    arr.emplace_back(playerName, score);

    scanScore(arr);

    boost::property_tree::ptree json;
    writeScore(arr, json);
    write_json(gkBasicScorePath, json);
  }

  void GameController::updateGameConditions(const int kAction) {
    player_.updateCondition(kAction, Trace_);
    for( auto& object: gameObjects_ ) {
      object->updateCondition(Trace_);
    }
  }

  void GameController::checkCollisions() {
    player_.action(gameObjects_, Trace_);
    for( auto& object: gameObjects_ ) {
      object->action(gameObjects_, Trace_);
    }
  }

  void GameController::updateGameRotations() {
    while( !player_.rotationEnd() ) {
      auto newCoords = player_.getNewCoords();
      auto coordsAllow = gscreen_.fixCollision(newCoords);
      player_.checkRoute(coordsAllow);
    }

    for( auto& object: gameObjects_ ) {
      while( !object->rotationEnd() ) {
        auto newCoords = object->getNewCoords();
        auto coordsAllow = gscreen_.fixCollision(newCoords);
        object->checkRoute(coordsAllow);
      }
    }
  }

  void GameController::updateGameContext(const int kAction) {
    updateGameConditions(kAction);
    updateGameRotations();
    checkCollisions();

    gscreen_.updateGameStat(gkBulletsField, player_.getAmmoQuantity());
    gscreen_.updateGameStat(gkPlayerLives, player_.getLivesQuantity());

    if( !player_.isAlive() ) {
      conditions_.PlayerIsDead = true;
    }

    /*if( gameObjects_.empty() ) {
      conditions_.GameIsEnd = true;
    }*/
  }

  void GameController::drawGameContext() {
    gscreen_.deleteObj(player_.getCoords());
    for( auto& obj: gameObjects_ ) {
      gscreen_.deleteObj(obj->getCoords());
    }

    auto deadBegin = std::remove_if(
            gameObjects_.begin(), gameObjects_.end(),
            [](std::shared_ptr<gameObj::ShiftingObject>& obj) { return !obj->isAlive(); });
    gameObjects_.erase(deadBegin, gameObjects_.end());

    gscreen_.drawObj(player_.getNewCoords(), player_.getAvatar());
    for( auto& obj: gameObjects_ ) {
      gscreen_.drawObj(obj->getNewCoords(), obj->getAvatar());
    }
  }

  GameController::GameController(gScreen::gameScreen& screen)
      : gscreen_(screen)
      , mapSize_(gscreen_.getMapSize())
      , player_(gameObj::ObjDirection::ekObjUp,
                std::pair{mapSize_.width / 2, mapSize_.height - 2}) {

    for( int pidorsQuantity = 0;
         pidorsQuantity < gkEnemyQuantity && pidorsQuantity < mapSize_.width; ++pidorsQuantity ) {
      gameObjects_.push_back(std::make_shared<gameObj::Enemy>(gameObj::ObjDirection::ekOBJDown,
                                                              std::pair{pidorsQuantity, 2}));
    }

    drawGameContext();
  }

  bool GameController::gameIsEnd() const {
    return conditions_.PlayerIsDead || conditions_.GameIsEnd;
  }

  TerminationConditions GameController::getTerminationConditions() const {
    return conditions_;
  }
}// namespace gameProcess
