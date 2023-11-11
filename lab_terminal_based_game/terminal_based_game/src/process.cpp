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
  const std::string kPlayerLives = "life rest :";
  const int gkEnemyQuantity = 10;

  const std::string gkDefPlayerName = "Nigga sweg";
  const std::string kGameName = "Nigga sweg";

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

      gscreen.fixCoordsToMove(objectCoords, desiredShift);
      a->makeShift(desiredShift);
      gscreen.drawMoveGameObj(objectCoords, desiredShift, a->avatar());
    }
  }

  gameProcessConstants gameProcess::updateEnemyWeapons(gScreen::gameScreen& gscreen,
                                                       shootMode& environmentInf) {
    for( auto weapon = environmentInf.enemyWeapons.begin();
         weapon != environmentInf.enemyWeapons.end(); ) {
      std::pair desiredShift{(*weapon)->desiredShift()};

      bool flag = gscreen.fixCoordsToMove((*weapon)->getCoords(), desiredShift);
      (*weapon)->makeShift(desiredShift);
      gscreen.drawMoveGameObj((*weapon)->getCoords(), desiredShift, (*weapon)->avatar());

      if( flag || (*weapon)->getCoords() == environmentInf.player.getCoords() ) {
        gscreen.deleteGameObj((*weapon)->getCoords());
        if( (*weapon)->getCoords() == environmentInf.player.getCoords() ) {
          --environmentInf.playerLives;
          if( !environmentInf.playerLives ) {
            return gameProcessConstants::ekPlayerDead;
          }
        }
        environmentInf.enemyWeapons.erase(weapon);
      }
      else {
        ++weapon;
      }
    }
    return ekNothingHappenedInTiananmenSquare;
  }

  void gameProcess::updateMyWeapons(gScreen::gameScreen& gscreen, shootMode& environmentInf) {
    for( auto weapon = environmentInf.myWeapons.begin();
         weapon != environmentInf.myWeapons.end(); ) {
      std::pair desiredShift{(*weapon)->desiredShift()};
      std::pair objectCoords{(*weapon)->getCoords()};

      bool flag = gscreen.fixCoordsToMove(objectCoords, desiredShift);
      (*weapon)->makeShift(desiredShift);
      gscreen.drawMoveGameObj(objectCoords, desiredShift, (*weapon)->avatar());

      for( auto object = environmentInf.gameObjects.begin();
           object != environmentInf.gameObjects.end(); ) {
        if( (*object)->getCoords() == (*weapon)->getCoords() ) {
          flag = true;
          gscreen.deleteGameObj((*object)->getCoords());
          environmentInf.gameObjects.erase(object);
        }
        else {
          ++object;
        }
      }

      if( flag ) {
        gscreen.deleteGameObj((*weapon)->getCoords());
        environmentInf.myWeapons.erase(weapon);
      }
      else {
        ++weapon;
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

    gscreen.fixCoordsToMove(objectCoords, desiredShift);
    environmentInf.player.makeShift(desiredShift);
    gscreen.drawMoveGameObj(objectCoords, desiredShift, environmentInf.player.avatar());
    // todo replace in screen
    std::string ammoQuantity = std::to_string(environmentInf.player.getAmmoQuantity());
    ammoQuantity.insert(0, 3 - ammoQuantity.length(), '0');
    gscreen.updateGameStat(gkBulletsField, std::move(ammoQuantity));
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


    while( (input = gscreen.screenInput()) != gkQuit && !gameEnvironment.gameObjects.empty() ) {
      if( input != ERR ) {
        updatePlayer(gscreen, gameEnvironment, input);
      }

      updateGameProcess(gscreen, gameEnvironment);
      updateMyWeapons(gscreen, gameEnvironment);
      updateEnemyWeapons(gscreen, gameEnvironment);

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


}// namespace gameProcess
