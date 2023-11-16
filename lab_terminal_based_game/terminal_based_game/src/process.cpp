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
  const int gkEnemyQuantity = 4;

  const std::string gkDefPlayerName = "unknown player";

  const std::string gkBasicGameMenuPath = "game_menu.json";
  const std::string gkBasicGameScreenSettingsPath = "game_screen.json";
  const std::string gkBasicScorePath = "score.json";

  const char *gkScoreTitle = "score";
  const char *gkScorePlayer = "player";
  const char *gkScoreScore = "score";
  const char *gkScoreMaxFieldsQuantity = "max_quantity";
  const char *gkScoreFields = "fields";


  /// for game controller
  const char *gkGameMapSettingPath = "game_map.json";
  const char *gkWidth = "width";
  const char *gkHeight = "height";
  const char *gkX0 = "x";
  const char *gkY0 = "y";
  const char *gkMap = "map";
    const char *gkEmptySpace = "empty_space";
}// namespace

namespace gameProcess {
  int gameProcess::showMenu(std::string& playerName) {
    gScreen::gameMenu menu{mainScreen_, gkBasicGameMenuPath, gkBasicScorePath};
    int input;
    while( (input = menu.input()) != gkQuit ) {
      if( menu.introducePlayerName(input) ) {
        break;
      }
    }
    playerName = menu.getPlayerName();
    return input;
  }

  int gameProcess::process() {
    while( mainScreen_.input() != gkQuit ) {
      std::string playerName{};

      if( showMenu(playerName) == gkQuit ) {
        break;
      }
      playerName = (playerName.empty()) ? gkDefPlayerName : playerName;
      startGame(gameMode::ekRateMode, playerName);
    }

    return 0;
  }

  void gameProcess::startRate(const std::string& kPlayerName) {
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
      auto gameEndTime = std::chrono::steady_clock::now();
      const int elapsedTime =
              std::chrono::duration_cast<std::chrono::seconds>(gameEndTime - startGameTime).count();
      updateScore(kPlayerName, elapsedTime);
    }
  }

  int gameProcess::startGame(gameMode mode, const std::string& kPlayerName) {
    if( mode == gameMode::ekRateMode ) {
      startRate(kPlayerName);
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
    }
  }

  void LolGameController::updateGameRotations() {
    while( !player_->rotationEnd() ) {
      auto newCoords = player_->offerNewCoords();
      auto coordsAllow = fixCollision(newCoords);
      player_->checkRoute(coordsAllow);
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

    if( gameObjects_.empty() ) {
      conditions_.GameIsEnd = true;
    }
  }

  void LolGameController::drawGameContext() {
    gscreen_.deleteObj(player_->getCoords());
    for( auto& obj: gameObjects_ ) {
      gscreen_.deleteObj(obj->getCoords());
    }

    auto deadBegin = std::remove_if(
            gameObjects_.begin(), gameObjects_.end(),
            [](std::shared_ptr<gameObj::ShiftingObject>& obj) { return !obj->isAlive(); });
    gameObjects_.erase(deadBegin, gameObjects_.end());

    gscreen_.drawObj(player_->getNewCoords(), player_->getAvatar());
    gameObjects_.insert(gameObjects_.end(), std::make_move_iterator(Trace_.begin()),
                        std::make_move_iterator(Trace_.end()));
    Trace_.clear();
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
    for( int pidorsQuantity = 0;
         pidorsQuantity < gkEnemyQuantity && pidorsQuantity < gameMapSize_.w; ++pidorsQuantity ) {
      gameObjects_.push_back(std::make_shared<gameObj::Enemy>(gameObj::ObjDirection::ekOBJDown,
                                                              std::pair{1+ pidorsQuantity, 2}));
    }
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
