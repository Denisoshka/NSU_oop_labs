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
  const int gkMinimalAmmoQuantity = 10;
  const char gkQuit{']'};
  const std::string gkBulletsField{"bullets quantity: "};
  const std::string gkElapsedTimeField{"elapsed time: "};
  const std::string kPlayerLives{"life rest :"};
  const int gkEnemyQuantity = 10;

  const std::string kDefPlayerName{"obezyana"};
  const std::string kGameName{"Nigga sweg"};

  const std::string gkBasicGameMenuPath{"game_menu.json"};
  const std::string gkBasicGameScreenPath{"game_screen.json"};
  const std::string gkBasicScorePath{"score.json"};
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

  gameProcessConstants gameProcess::updateEnemyWeapons(gScreen::gameScreen& gscreen, shootMode& environmentInf ){
    for( auto weapon = environmentInf.enemyWeapons.begin(); weapon != environmentInf.enemyWeapons.end(); ) {
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
    for( auto weapon = environmentInf.myWeapons.begin(); weapon != environmentInf.myWeapons.end(); ) {
      std::pair desiredShift{(*weapon)->desiredShift()};
      std::pair objectCoords{(*weapon)->getCoords()};

      bool flag = gscreen.fixCoordsToMove(objectCoords, desiredShift);
      (*weapon)->makeShift(desiredShift);
      gscreen.drawMoveGameObj(objectCoords, desiredShift, (*weapon)->avatar());

      for( auto object = environmentInf.gameObjects.begin(); object != environmentInf.gameObjects.end(); ) {
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
//todo replace in screen
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

      startGame();



    }

    return 0;
  }

  int gameProcess::startGame() {
    gScreen::gameScreen gscreen{mainScreen_, gkBasicGameScreenPath};

    shootMode gameEnvironment;
    srandom(time(nullptr));
    auto startGameTime = std::chrono::steady_clock::now();
    int input;

    initShootModeEnvironment(gscreen, gameEnvironment);


    while( (input = gscreen.screenInput()) != gkQuit && !gameEnvironment.gameObjects.empty() ) {
      if( input != ERR ) {
        updatePlayer(gscreen,gameEnvironment, input);
      }

      updateGameProcess(gscreen,gameEnvironment);
      updateMyWeapons(gscreen,gameEnvironment);
      updateEnemyWeapons(gscreen,gameEnvironment);

      auto end = std::chrono::steady_clock::now();
      auto seconds = std::to_string(std::chrono::duration_cast<std::chrono::seconds>(end - startGameTime).count());
      gscreen.updateGameStat(gkElapsedTimeField, std::move(seconds));
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    auto gameEndTime = std::chrono::steady_clock::now();
    startGameTime += gameEndTime - startGameTime;

    return 0;
  }

}// namespace gameProcess
