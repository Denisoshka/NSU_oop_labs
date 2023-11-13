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
  const std::string kPlayerLives = "lives: ";
  const int gkEnemyQuantity = 10;

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
  void gameProcess::initShootModeEnvironment(gScreen::gameScreen& gscreen,
                                             shootMode& environmentInf) {
    gScreen::windowSettings mapSize = gscreen.GetMapSize();
    std::pair playerCords1 = std::pair{mapSize.width / 2, mapSize.height - 2};
    std::pair playerCords2 = playerCords1;
    environmentInf.player = gameObj::Player{gameObj::ekObjUp, std::move(playerCords1)};

    for( int i = 0; i < gkEnemyQuantity; ++i ) {
      environmentInf.gameObjects.push_back(std::make_shared<gameObj::Enemy>(
              gameObj::ObjDirection::ekOBJDown, std::pair{mapSize.width / 2, 2}));
    }

    gscreen.drawMoveGameObj(playerCords2, std::pair{0, 0}, environmentInf.player.avatar());
  }

  void gameProcess::updateGameProcess(gScreen::gameScreen& gscreen, shootMode& environmentInf) {
    for( const auto& a: environmentInf.gameObjects ) {
      std::pair desiredShift = a->desiredShift();
      std::pair objectCoords = a->getCoords();

      auto action = a->action('_');
      if( action ) {
        environmentInf.enemyWeapons.push_back(std::move(action));
      }

      gscreen.fixCollision(objectCoords, desiredShift);
      a->makeShift(desiredShift);
      gscreen.drawMoveGameObj(objectCoords, desiredShift, a->avatar());
    }
  }

  auto gameProcess::eraseProcessObject(
          gScreen::gameScreen& gscreen, auto objectIterator,
          std::vector<std::shared_ptr<gameObj::ShiftingObject>>& objects) {
    gscreen.deleteObj((*objectIterator)->getCoords());
    return objects.erase(objectIterator);
  }

  void gameProcess::updateEnemyWeapons(gScreen::gameScreen& gscreen, shootMode& environmentInf) {
    for( auto weapon = environmentInf.enemyWeapons.begin();
         weapon != environmentInf.enemyWeapons.end(); ) {

      std::pair desiredShift{(*weapon)->desiredShift()};
      bool isCollision = gscreen.fixCollision((*weapon)->getCoords(), desiredShift);
      (*weapon)->makeShift(desiredShift);
      gscreen.drawMoveGameObj((*weapon)->getCoords(), desiredShift, (*weapon)->avatar());

      if( isCollision && !desiredShift.first && !desiredShift.second ) {
        weapon = eraseProcessObject(gscreen, weapon, environmentInf.enemyWeapons);
      }
      else {
        ++weapon;
      }
    }
  }

  void gameProcess::checkMyCollisions(gScreen::gameScreen& gscreen, shootMode& environmentInf) {
    for( auto weapon = environmentInf.enemyWeapons.begin();
         weapon != environmentInf.enemyWeapons.end() && environmentInf.player.isAlive(); ) {
      if( environmentInf.player.isCollision(**weapon) ) {
        environmentInf.player.battle(**weapon);
      }
      if( !(*weapon)->isAlive() ) {
        weapon = eraseProcessObject(gscreen, weapon, environmentInf.enemyWeapons);
      }
      else {
        weapon++;
      }
    }
    gscreen.updateGameStat(kPlayerLives, environmentInf.player.getLivesQuantity());
  }

  void gameProcess::updateMyWeapons(gScreen::gameScreen& gscreen, shootMode& environmentInf) {
    for( auto myWeapon = environmentInf.myWeapons.begin();
         myWeapon != environmentInf.myWeapons.end(); ) {

      std::pair desiredShift{(*myWeapon)->desiredShift()};
      bool isCollision = gscreen.fixCollision((*myWeapon)->getCoords(), desiredShift);
      gscreen.drawMoveGameObj((*myWeapon)->getCoords(), desiredShift, (*myWeapon)->avatar());
      (*myWeapon)->makeShift(desiredShift);

      if( (isCollision && !desiredShift.first && !desiredShift.second) ) {
        myWeapon = eraseProcessObject(gscreen, myWeapon, environmentInf.myWeapons);
      }
      else {
        ++myWeapon;
      }
    }
  }

  void gameProcess::checkEnemyCollisions(gScreen::gameScreen& gscreen, shootMode& environmentInf) {
    for( auto myWeapon = environmentInf.myWeapons.begin();
         myWeapon != environmentInf.myWeapons.end(); ) {
      for( auto object = environmentInf.gameObjects.begin();
           object != environmentInf.gameObjects.end() && (**myWeapon).isAlive(); ) {
        if( (**object).isCollision(**myWeapon) && (**object).battle(**myWeapon) ) {
          object = eraseProcessObject(gscreen, object, environmentInf.gameObjects);
        }
        else {
          object++;
        }
      }

      if( (!(**myWeapon).isAlive()) ) {
        myWeapon = eraseProcessObject(gscreen, myWeapon, environmentInf.myWeapons);
      }
      else {
        ++myWeapon;
      }
    }
  }

  void gameProcess::updatePlayer(gScreen::gameScreen& gscreen, shootMode& environmentInf,
                                 const int action) {
    std::shared_ptr<gameObj::ShiftingObject> playerAction = environmentInf.player.action(action);
    if( playerAction != nullptr ) {
      environmentInf.myWeapons.push_back(playerAction);
    }
    std::pair desiredShift = environmentInf.player.desiredShift();
    std::pair objectCoords = environmentInf.player.getCoords();

    gscreen.fixCollision(objectCoords, desiredShift);
    gscreen.drawMoveGameObj(objectCoords, desiredShift, environmentInf.player.avatar());
    environmentInf.player.makeShift(desiredShift);

    gscreen.updateGameStat(gkBulletsField, environmentInf.player.getAmmoQuantity());
  }

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
    shootMode gameEnvironment;
    initShootModeEnvironment(gscreen, gameEnvironment);
    srandom(time(nullptr));
    auto startGameTime = std::chrono::steady_clock::now();
    int input;


    while( (input = gscreen.screenInput()) != gkQuit && !gameEnvironment.gameObjects.empty()
           && gameEnvironment.player.isAlive() ) {
      if( input != ERR ) {
        updatePlayer(gscreen, gameEnvironment, input);
      }

      updateGameProcess(gscreen, gameEnvironment);
      updateMyWeapons(gscreen, gameEnvironment);
      updateEnemyWeapons(gscreen, gameEnvironment);
      checkMyCollisions(gscreen, gameEnvironment);
      checkEnemyCollisions(gscreen, gameEnvironment);

      auto end = std::chrono::steady_clock::now();
      auto seconds = std::to_string(
              std::chrono::duration_cast<std::chrono::seconds>(end - startGameTime).count());
      gscreen.updateGameStat(gkElapsedTimeField, std::move(seconds));
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    if( gameEnvironment.gameObjects.empty() ) {
      auto gameEndTime = std::chrono::steady_clock::now();
      const int elapsedTime =
              std::chrono::duration_cast<std::chrono::seconds>(gameEndTime - startGameTime).count();
      updateScore(kPlayerName, elapsedTime);
    }
  }

  void gameProcess::startTraining(gScreen::gameScreen& gscreen) {
  }

  int gameProcess::startGame(gameMode mode, const std::string& kPlayerName) {
    gScreen::gameScreen gscreen{mainScreen_, gkBasicGameScreenPath};
    if( mode == gameMode::ekRateMode ) {
      startRate(gscreen, kPlayerName);
    }
    else if( mode == gameMode::ekTrainingMode ) {
      startTraining(gscreen);
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

  void gameProcess::GameController::calculateConditions(std::vector<std::shared_ptr<gameObj::ShiftingObject>> & trace){
    std::vector<std::shared_ptr<gameObj::ShiftingObject>> newItems{};
    for (auto & object : gameObjects_){

    }
  }

  void gameProcess::GameController::updateGameContext(std::vector<std::shared_ptr<gameObj::ShiftingObject>> & trace) {
    std::vector<std::shared_ptr<gameObj::ShiftingObject>> newItems{};
    for (auto & object : gameObjects_){
      object->updateCondition(newItems);
    }
    std::move(newItems.begin(), newItems.end(), std::back_inserter(gameObjects_));
  }

  void gameProcess::GameController::drawGameContext() {
    for (auto & object : gameObjects_){
      if ()
    }
  }
}// namespace gameProcess
