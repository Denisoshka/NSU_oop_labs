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
}

namespace gameProcess {
  int screenInput(WINDOW *window) {
    int input = wgetch(window);
    return input;
  }

  const char kEnd{'q'};

  int gameProcess::process() {
    std::vector<int> mapSize{mapSize_.width, mapSize_.height, 0, 0};
    std::vector<int> statsSize{mapSize_.width, 6, 0, mapSize_.height};

    gameScreen::gameScreen screen{mapSize, statsSize};
    screen.loadGameMap(std::move(gameMap_), ' ');
    screen.drawGameMap();

    std::vector<std::shared_ptr<gameObj::ShiftingObject>> gameObjects{};
    std::vector<std::shared_ptr<gameObj::ShiftingObject>> wearpons{};
    wearpons.reserve(minimalAmmoQuantity);

    gameObj::Player player{
            gameObj::ekObjUp, std::pair{mapSize_.width / 2, mapSize_.height - 2}
    };

    while( true ) {
      std::future<int> futureInput =
              std::async(std::launch::async, screenInput, screen.getWindow());

      while( futureInput.wait_for(std::chrono::nanoseconds(10000)) != std::future_status::ready ) {
        /// здесь вдигаются все живие объекты;
        /*for( const auto& a: gameObjects ) {
          std::pair desiredShift = a->desiredShift();
          std::pair objectCoords = a->getCoords();

          screen.fixCoordsToMove(objectCoords, desiredShift);
          a->makeShift(desiredShift);
          screen.drawGameObj(objectCoords, desiredShift, a->avatar());
        }*/
        for( int i = 0; i < wearpons.size(); i++ ) {
          std::pair desiredShift{wearpons[i]->desiredShift()};
          std::pair objectCoords{wearpons[i]->getCoords()};
          bool flag = screen.fixCoordsToMove(objectCoords, desiredShift);
          screen.drawGameObj(objectCoords, desiredShift, wearpons[i]->avatar());
          for (const auto & obejct: gameObjects){
            if (obejct->getCoords() == wearpons[i]->getCoords()){

            }
          }
          if (flag){
            wearpons.erase(i);
          }
        }
        /// здесь будем вроверять попали ли пули;
      }

      if( futureInput.valid() ) {
        /// здесь будем играть персонажем
        int action = futureInput.get();
        if( action == kEnd ) {
          break;
        }

        std::shared_ptr<gameObj::ShiftingObject> playerAction = player.action(action);
        if( playerAction != nullptr ) {
          wearpons.push_back(playerAction);
        }
        std::pair desiredShift = player.desiredShift();
        std::pair objectCoords = player.getCoords();

        screen.fixCoordsToMove(objectCoords, desiredShift);
        player.makeShift(desiredShift);
        screen.drawGameObj(objectCoords, desiredShift, player.avatar());
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
