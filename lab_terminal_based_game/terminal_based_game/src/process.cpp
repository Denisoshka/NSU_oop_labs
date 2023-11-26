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
  const int gkEnemyQuantity = 3;
  const char gkQuit{']'};
  const std::string gkBulletsField = "bullets quantity: ";
  const std::string gkElapsedTimeField = "elapsed time: ";
  const std::string gkPlayerLives = "lives: ";

  const std::string gkDefPlayerName = "unknown player";

  const std::string gkBasicGameMenuPath = "game_menu.json";
  const std::string gkBasicGameScreenSettingsPath = "game_screen.json";
  const std::string gkBasicScorePath = "score.json";

  const std::string gkEnemy1 = "enemy_1";
  const std::string gkEnemy2 = "enemy_2";
  const std::string gkEnemy3 = "enemy_3";
  const float gkEnemy1ScoreCoef = 2;
  const float gkEnemy2ScoreCoef = 1;
  const float gkEnemy3ScoreCoef = 3;

  const std::string gkScoreTitle = "score";
  const std::string gkScorePlayer = "player";
  const std::string gkScoreScore = "score";
  const std::string gkScoreMaxFieldsQuantity = "max_quantity";
  const std::string gkScoreFields = "fields";

  /// for game controller
  const std::string gkGameMapSettingPath = "game_map.json";
  const std::string gkWidth = "width";
  const std::string gkHeight = "height";
  const std::string gkX0 = "x";
  const std::string gkY0 = "y";
  const std::string gkMap = "map";
  const std::string gkEmptySpace = "empty_space";
}// namespace

namespace gameProcess {
  int gameProcess::showMenu(std::tuple<std::string, std::vector<int>>& introducedInf) {
    gScreen::gameMenu menu{mainScreen_, gkBasicGameMenuPath, gkBasicScorePath};
    int input = menu.process();
    introducedInf = menu.getIntroducedInf();
    return input;
  }

  int gameProcess::process() {
    while( mainScreen_.input() != gkQuit ) {
      std::string playerName{};
      std::tuple<std::string, std::vector<int>> introducedInf;
      if( showMenu(introducedInf) == gkQuit ) {
        break;
      }
      playerName = (playerName.empty()) ? gkDefPlayerName : playerName;
      startGame(gameMode::ekRateMode, introducedInf);
    }

    return 0;
  }

  void gameProcess::startRate(const std::tuple<std::string, std::vector<int>>& introducedInf) {
    srandom(time(nullptr));
    auto startGameTime = std::chrono::steady_clock::now();
    int input;

    LolGameController controller{
            coreScreen_,
            gkBasicGameScreenSettingsPath,
            gkGameMapSettingPath,
    };

    while( (input = controller.input()) != gkQuit && !controller.gameIsEnd() ) {

      controller.updateGameContext(input);
      controller.drawGameContext();

      auto end = std::chrono::steady_clock::now();
      auto seconds = std::chrono::duration_cast<std::chrono::seconds>(end - startGameTime).count();
      controller.updateStat(gkElapsedTimeField, seconds);
    }

    if( controller.getTerminationConditions().GameIsEnd ) {
      updateScore(std::get<0>(introducedInf), std::get<1>(introducedInf));
    }
  }

  int gameProcess::startGame(gameMode mode,
                             const std::tuple<std::string, std::vector<int>>& introducedInf) {
    if( mode == gameMode::ekRateMode ) {
      startRate(introducedInf);
    }

    return 0;
  }

  static void scanScore(std::vector<std::pair<std::string, std::vector<int>>>& arr) {
    boost::property_tree::ptree scores;
    boost::property_tree::read_json(gkBasicScorePath, scores);

    for( auto row = scores.get_child(gkScoreTitle).get_child(gkScoreFields).begin();
         row != scores.get_child(gkScoreTitle).get_child(gkScoreFields).end(); ++row ) {
      arr.emplace_back(row->second.get<std::string>(gkScorePlayer),
                       std::vector{row->second.get<int>(gkEnemy1), row->second.get<int>(gkEnemy2),
                                   row->second.get<int>(gkEnemy3)});
    }

    std::sort(arr.begin(), arr.end(), [](auto& left, auto& right) {
      return (gkEnemy1ScoreCoef * left.second[0] + gkEnemy2ScoreCoef * left.second[1]
              + gkEnemy3ScoreCoef * left.second[2])
           < (gkEnemy1ScoreCoef * right.second[0] + gkEnemy2ScoreCoef * right.second[1]
              + gkEnemy3ScoreCoef * right.second[2]);
    });

    arr.resize(std::min<size_t>(gkMaxFieldsQuantity, arr.size()));
  }

  static void writeScore(std::vector<std::pair<std::string, std::vector<int>>>& arr,
                         boost::property_tree::ptree& json) {
    boost::property_tree::ptree newScore;
    boost::property_tree::ptree jsonScoreFields;

    for( const auto& values: arr ) {
      boost::property_tree::ptree player;
      player.put(gkScorePlayer, values.first);
      player.put(gkEnemy1, values.second[0]);
      player.put(gkEnemy1, values.second[1]);
      player.put(gkEnemy1, values.second[2]);
      jsonScoreFields.push_back({"", player});
    }

    newScore.put(gkScoreMaxFieldsQuantity, gkMaxFieldsQuantity);
    newScore.add_child(gkScoreFields, jsonScoreFields);
    json.add_child(gkScoreScore, newScore);
  }

  void gameProcess::updateScore(const std::string& playerName, const std::vector<int>& enemy) {
    std::vector<std::pair<std::string, std::vector<int>>> arr{};
    arr.emplace_back(playerName, enemy);

    scanScore(arr);

    boost::property_tree::ptree json;
    writeScore(arr, json);
    write_json(gkBasicScorePath, json);
  }

  void LolGameController::updateGameConditions(const int kAction) {
    player_->updateCondition(kAction, Trace_);
    for( auto& object: gameObjects_ ) {
      object->updateCondition(Trace_);
    }
  }

  void LolGameController::checkCollisions() {
    player_->action(gameObjects_, Trace_);
    for( auto& object: gameObjects_ ) {
      object->action(gameObjects_, Trace_);
      object->action(playerObjects, Trace_);
    }
  }

  void LolGameController::updateGameRotations() {
    for( auto& playerObj: playerObjects ) {
      while( !playerObj->rotationEnd() ) {
        auto newCoords = playerObj->offerNewCoords();
        auto coordsAllow = fixCollision(newCoords);
        playerObj->checkRoute(coordsAllow);
      }
    }

    for( auto& object: gameObjects_ ) {
      while( !object->rotationEnd() ) {
        auto newCoords = object->offerNewCoords();
        auto coordsAllow = fixCollision(newCoords);
        object->checkRoute(coordsAllow);
      }
    }
  }

  void LolGameController::updateGameContext(const int kAction) {
    updateGameConditions(kAction);
    updateGameRotations();
    checkCollisions();

    gscreen_.updateGameStat(gkBulletsField, player_->getAmmoQuantity());
    gscreen_.updateGameStat(gkPlayerLives, player_->getLivesQuantity());

    if( !player_->isAlive() ) {
      conditions_.PlayerIsDead = true;
    }

    auto deadBegin = std::remove_if(
            endGameIndicator_.begin(), endGameIndicator_.end(),
            [](std::weak_ptr<gameObj::ShiftingObject>& obj) { return obj.expired(); });
    endGameIndicator_.erase(deadBegin, endGameIndicator_.end());

    if( endGameIndicator_.empty() ) {
      conditions_.GameIsEnd = true;
    }
  }

  void LolGameController::drawGameContext() {
    for( auto& playerObj: playerObjects ) {
      gscreen_.deleteObj(playerObj->getCoords());
    }
    for( auto& obj: gameObjects_ ) {
      gscreen_.deleteObj(obj->getCoords());
    }

    auto deadBegin = std::remove_if(
            gameObjects_.begin(), gameObjects_.end(),
            [](std::shared_ptr<gameObj::ShiftingObject>& obj) { return !obj->isAlive(); });
    gameObjects_.erase(deadBegin, gameObjects_.end());

    gameObjects_.insert(gameObjects_.end(), std::make_move_iterator(Trace_.begin()),
                        std::make_move_iterator(Trace_.end()));
    Trace_.clear();

    for( auto& playerObj: playerObjects ) {
      gscreen_.drawObj(playerObj->getNewCoords(), playerObj->getAvatar());
    }

    for( auto& obj: gameObjects_ ) {
      gscreen_.drawObj(obj->getNewCoords(), obj->getAvatar());
    }
  }

  BasicGameController::BasicGameController(const gScreen::BasicScreen& kScreen,
                                           const std::string& kGameScreenSettings,
                                           const std::string& kGameMapSettings)
      : gscreen_(gScreen::GameScreen{kScreen, kGameScreenSettings, gameMap_, gameMapSize_}) {
    loadGameMap(kGameMapSettings);
    gscreen_.updateMapSettings();
  }

  void BasicGameController::loadGameMap(const std::string& kGameMapSettings) {
    boost::property_tree::ptree ptreeMapSettings;
    boost::property_tree::read_json(kGameMapSettings, ptreeMapSettings);
    gameMapSize_ = gScreen::windowSettings{
            .w = ptreeMapSettings.get<int>(gkWidth),
            .h = ptreeMapSettings.get<int>(gkHeight),
            .X0 = ptreeMapSettings.get<int>(gkX0, 0),
            .Y0 = ptreeMapSettings.get<int>(gkY0, 0),
    };
    //    todo i dont know how to use X0
    gameMap_ = ptreeMapSettings.get<std::string>(gkMap);
    emptySpace_ = ptreeMapSettings.get<char>(gkEmptySpace, ' ');
  }

  void LolGameController::initGameContext() {
    playerObjects.push_back(player_);
    for( int enemyQuantity = 0; enemyQuantity < gkEnemyQuantity && enemyQuantity < gameMapSize_.w;
         ++enemyQuantity ) {
      auto defenemy = std::make_shared<gameObj::Enemy>(gameObj::ObjDirection::ekOBJDown,
                                                       std::pair{(1 + enemyQuantity) * 5, 2});
      gameObjects_.push_back(defenemy);
      endGameIndicator_.push_back(std::weak_ptr(defenemy));
    }
    auto barricade = std::make_shared<gameObj::DroneGenerator>(
            gameObj::ObjDirection::ekOBJDown, std::pair{10, 4},
            gameObj::ObjectFraction::ekEnemyFraction, '-');
    gameObjects_.push_back(barricade);
    endGameIndicator_.push_back(barricade);
    auto tracking = std::make_shared<gameObj::TrackingEnemy>(gameObj::ObjDirection::ekOBJDown,
                                                             std::pair{10, 5});
    gameObjects_.push_back(tracking);
    endGameIndicator_.push_back(barricade);
  }

  LolGameController::LolGameController(const gScreen::BasicScreen& kScreen,
                                       const std::string& kGameScreenSettings,
                                       const std::string kGameMapSettings)
      : BasicGameController(kScreen, kGameScreenSettings, kGameMapSettings) {
    player_ = std::make_shared<gameObj::Player>(gameObj::ObjDirection::ekObjUp,
                                                std::pair{gameMapSize_.w / 2, gameMapSize_.h - 2});
    LolGameController::initGameContext();
    LolGameController::drawGameContext();
    gscreen_.drawGameMap();
  }

  std::vector<std::pair<bool, bool>> LolGameController::fixCollision(
          const std::vector<std::pair<int, int>>& Route) {
    std::vector<std::pair<bool, bool>> routeAllow{};
    for( auto& RouteBlock: Route ) {
      // todo look here for collisions;
      int desiredX = std::clamp(RouteBlock.first, gameMapSize_.X0, gameMapSize_.w - 2);
      int desiredY = std::clamp(RouteBlock.second, gameMapSize_.Y0, gameMapSize_.h - 2);
      routeAllow.emplace_back(desiredX == RouteBlock.first && RouteBlock.second == desiredY,
                              gameMap_[desiredX + desiredY * gameMapSize_.w] == emptySpace_);
    }
    return routeAllow;
  }

  bool LolGameController::gameIsEnd() const {
    return conditions_.PlayerIsDead || conditions_.GameIsEnd;
  }

  TerminationConditions LolGameController::getTerminationConditions() const {
    return conditions_;
  }

  bool LolGameController::isEmptySpase(const int x) {
    return x == emptySpace_;
  }

  int LolGameController::input() {
    return gscreen_.input();
  }

  void LolGameController::updateStat(const std::string& kField, const int kValue) {
    gscreen_.updateGameStat(kField, kValue);
  }
}// namespace gameProcess
