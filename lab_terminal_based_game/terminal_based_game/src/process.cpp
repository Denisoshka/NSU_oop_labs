#include "process.hpp"
#include "game_obj.hpp"
#include "game_screen.hpp"
#include "shifting_object.hpp"


#include <curses.h>
#include <chrono>
#include <cstdio>
#include <fstream>
#include <future>
#include <iostream>
#include <map>
#include <set>
#include <thread>
#include <unordered_map>

namespace {
  const int minimalAmmoQuantity{10};
  const char freeSpace{' '};
  const char kEnd{'q'};
  const std::string bulletsField{"bullets quantity: "};
  const std::string elapsedTimeField{"elapsed time: "};
}// namespace

namespace gameProcess {
  int screenInput(WINDOW *window) {
    int input{wgetch(window)};
    return input;
  }

  void gameProcess::initGameScreen(
          std::vector<std::pair<std::pair<int, int>, std::string>>&& stats) {
    std::vector<int> mapSize{mapSize_.width, mapSize_.height, 0, 0};
    std::vector<int> statsSize{mapSize_.width, 6, 0, mapSize_.height};

    screen_ = gameScreen::gameScreen{mapSize, statsSize};
    screen_.loadGameMap(std::move(gameMap_), freeSpace);
    screen_.drawGameMap();
    screen_.loadGameStats(std::move(stats));
    screen_.drawGameStats();
  }

  void gameProcess::initGameProcessEnvironment() {
    myWeapons.reserve(minimalAmmoQuantity);
    enemyWeapons.reserve(minimalAmmoQuantity);

    std::pair playerCords1 = std::pair{mapSize_.width / 2, mapSize_.height - 2};
    std::pair playerCords2 = playerCords1;
    player = gameObj::Player{gameObj::ekObjUp, std::move(playerCords1)};

    screen_.drawMoveGameObj(playerCords2, std::pair{0, 0}, player.avatar());
  }

  void gameProcess::updateGameEnvironment() {
    for( auto weapon = myWeapons.begin(); weapon != myWeapons.end(); ) {
      std::pair desiredShift{(*weapon)->desiredShift()};
      std::pair objectCoords{(*weapon)->getCoords()};

      bool flag = screen_.fixCoordsToMove(objectCoords, desiredShift);
      (*weapon)->makeShift(desiredShift);
      screen_.drawMoveGameObj(objectCoords, desiredShift, (*weapon)->avatar());

      for( const auto& object: gameObjects ) {
        if( object->getCoords() == (*weapon)->getCoords() ) {
          flag = true;
        }
      }
      if( flag ) {
        screen_.deleteGameObj((*weapon)->getCoords());
        myWeapons.erase(weapon);
      }
      else {
        ++weapon;
      }
    }
  }

  void gameProcess::updatePlayer(const int action) {
    std::shared_ptr<gameObj::ShiftingObject> playerAction = player.action(action);
    if( playerAction != nullptr ) {
      myWeapons.push_back(playerAction);
    }
    std::pair desiredShift = player.desiredShift();
    std::pair objectCoords = player.getCoords();

    screen_.fixCoordsToMove(objectCoords, desiredShift);
    player.makeShift(desiredShift);
    screen_.drawMoveGameObj(objectCoords, desiredShift, player.avatar());

    std::string ammoQuantity = std::to_string(player.getAmmoQuantity());
    ammoQuantity.insert(0, 3 - ammoQuantity.length(), '0');
    screen_.updateGameStat(bulletsField, std::move(ammoQuantity));
  }

  int gameProcess::process() {
    std::vector<std::pair<std::pair<int, int>, std::string>> stats = {
            {{0, mapSize_.height + 1}, bulletsField    },
            {{0, mapSize_.height + 3}, elapsedTimeField}
    };

    initGameScreen(std::move(stats));
    initGameProcessEnvironment();
    auto start = std::chrono::steady_clock::now();
    gameObjects.push_back(
            std::make_shared<gameObj::Enemy>(gameObj::ObjDirection::ekOBJDown, std::pair{20, 2}));
    auto& enemy = gameObjects.front();
    screen_.drawMoveGameObj(enemy->getCoords(), enemy->desiredShift(), enemy->avatar());
    while( true ) {
      std::future<int> futureInput =
              std::async(std::launch::async, screenInput, screen_.getWindow());

      while( futureInput.wait_for(std::chrono::milliseconds(10)) != std::future_status::ready ) {
        /// здесь вдигаются все живие объекты;
         for( const auto& a: gameObjects ) {
           std::pair desiredShift = a->desiredShift();
           std::pair objectCoords = a->getCoords();

           auto action = a->action('0');
           if( action ) {
             enemyWeapons.push_back(std::move(action));
           }
           screen_.fixCoordsToMove(objectCoords, desiredShift);
           a->makeShift(desiredShift);
           screen_.drawMoveGameObj(objectCoords, desiredShift, a->avatar());
           }

        ///

        updateGameEnvironment();

                auto end = std::chrono::steady_clock::now();
                auto seconds = std::to_string(std::chrono::duration_cast<std::chrono::seconds>(end
                - start).count()); screen_.updateGameStat(elapsedTimeField, std::move(seconds));
      }

      if( futureInput.valid() ) {
        /// здесь будем играть персонажем
        int action = futureInput.get();
        if( action == kEnd ) {
          break;
        }
        updatePlayer(action);
      }
    }

    return 0;
  }

  gameProcess::gameProcess::gameProcess(std::string&& gameSettings) {
    std::ifstream mapInput{gameSettings};
    if( !mapInput.is_open() ) {
      // todo
    }
    mapInput >> mapSize_.width >> mapSize_.height;
    gameMap_.resize(mapSize_.width * mapSize_.height);
    char skipNextLineSymbol;
    mapInput.read(&skipNextLineSymbol, sizeof(skipNextLineSymbol));
    if( skipNextLineSymbol != '\n' ) {
      throw std::exception();//        todo
    }
    for( int row = 0; row < mapSize_.height; ++row ) {
      mapInput.read(reinterpret_cast<char *>(gameMap_.data() + row * mapSize_.width),
                    mapSize_.width * sizeof(*gameMap_.data()));
      mapInput.read(&skipNextLineSymbol, sizeof(skipNextLineSymbol));
      if( skipNextLineSymbol != '\n' ) {
        throw std::exception();//        todo
      }
    }
  }


}// namespace gameProcess
