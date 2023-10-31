#include "process.hpp"
#include "game_obj.hpp"
#include "game_screen.hpp"
#include "shifting_object.hpp"

#include <curses.h>
#include <chrono>
#include <fstream>
#include <future>
#include <iostream>
#include <random>
#include <set>

namespace {
  const int minimalAmmoQuantity{10};
  const char freeSpace{' '};
  const char kFinishGame{']'};
  const std::string_view bulletsField{"bullets quantity: "};
  const std::string_view kElapsedTimeField{"elapsed time: "};
  const std::string_view kPlayerLives{"life rest :"};
  const int kEnemyQuantity{10};
  const int kStatsQuantity{10};
  const std::string kGameStats{"game_stats.txt"};
  const std::string kGameName{"Nigga sweg"};
}// namespace

namespace gameProcess {
  int screenInput(WINDOW *window) {
    int input{wgetch(window)};
    return input;
  }

  /* static bool getRandomBoolean(double probability) {
     std::random_device rd;
     std::mt19937 gen(rd());
     std::uniform_real_distribution<double> dis(0.0, 1.0);

     double randomValue = dis(gen);

     return (randomValue < probability);
   }
 */
  void gameProcess::initGameScreen(
          std::vector<std::pair<std::pair<int, int>, std::string_view>>&& stats) {
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
    for( int i = 0; i < kEnemyQuantity; ++i ) {
      gameObjects.push_back(std::make_shared<gameObj::Enemy>(gameObj::ObjDirection::ekOBJDown,
                                                             std::pair{mapSize_.width / 2, 2}));
    }
    screen_.drawMoveGameObj(playerCords2, std::pair{0, 0}, player.avatar());
  }

  void gameProcess::updateGameProcess() {
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
  }

  gameProcessConstants gameProcess::updateEnemyWeapons() {
    for( auto weapon = enemyWeapons.begin(); weapon != enemyWeapons.end(); ) {
      std::pair desiredShift{(*weapon)->desiredShift()};
      std::pair objectCoords{(*weapon)->getCoords()};

      bool flag = screen_.fixCoordsToMove(objectCoords, desiredShift);
      (*weapon)->makeShift(desiredShift);
      screen_.drawMoveGameObj(objectCoords, desiredShift, (*weapon)->avatar());

      if( flag || (*weapon)->getCoords() == player.getCoords() ) {
        screen_.deleteGameObj((*weapon)->getCoords());
        if( (*weapon)->getCoords() == player.getCoords() ) {
          playerLives--;
          if( !playerLives ) {
            return gameProcessConstants::ekPlayerDead;
          }
        }
        enemyWeapons.erase(weapon);
      }
      else {
        ++weapon;
      }
    }
    return ekNothingHappenedInTiananmenSquare;
  }

  void gameProcess::updateMyWeapons() {
    for( auto weapon = myWeapons.begin(); weapon != myWeapons.end(); ) {
      std::pair desiredShift{(*weapon)->desiredShift()};
      std::pair objectCoords{(*weapon)->getCoords()};

      bool flag = screen_.fixCoordsToMove(objectCoords, desiredShift);
      (*weapon)->makeShift(desiredShift);
      screen_.drawMoveGameObj(objectCoords, desiredShift, (*weapon)->avatar());

      for( auto object = gameObjects.begin(); object != gameObjects.end(); ) {
        if( (*object)->getCoords() == (*weapon)->getCoords() ) {
          flag = true;
          screen_.deleteGameObj((*object)->getCoords());
          gameObjects.erase(object);
        }
        else {
          object++;
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

  void gameProcess::updateGameEnvironment() {
    // todo
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
    srandom(time(nullptr));
    std::vector<std::pair<std::pair<int, int>, std::string_view>> stats = {
            {{0, mapSize_.height + 1}, bulletsField     },
            {{0, mapSize_.height + 3}, kElapsedTimeField}
    };


    bool exitFlag{false};
    while( true ) {

      while( true ) {
        int action;
        if( (action = screenInput(screen_.getWindow())) == kFinishGame ) {
          exitFlag = true;
          break;
        }
        else {
        }
      }
      if( exitFlag ) {
        break;
      }

      initGameScreen(std::move(stats));
      initGameProcessEnvironment();
      auto startGameTime = std::chrono::steady_clock::now();
      while( true ) {
        std::future<int> futureInput =
                std::async(std::launch::async, screenInput, screen_.getWindow());
        if( gameObjects.empty() ) {
          auto gameEndTime = std::chrono::steady_clock::now();
          startGameTime += gameEndTime - startGameTime;
          break;
        }

        while( futureInput.wait_for(std::chrono::milliseconds(10)) != std::future_status::ready ) {
          /// здесь вдигаются все живие объекты;
          updateGameProcess();
          updateMyWeapons();
          updateEnemyWeapons();
          ///

          auto end = std::chrono::steady_clock::now();
          auto seconds = std::to_string(
                  std::chrono::duration_cast<std::chrono::seconds>(end - startGameTime).count());
          screen_.updateGameStat(kElapsedTimeField, std::move(seconds));
        }

        if( futureInput.valid() ) {
          /// здесь будем играть персонажем
          int action = futureInput.get();
          if( action == kFinishGame ) {
            exitFlag = true;
            break;
          }
          updatePlayer(action);
        }
      }
    }

    return 0;
  }

  gameProcess::gameProcess::gameProcess(std::string&& gameSettings)
      : gameSettings_(std::move(gameSettings)) {
  }

  void gameProcess::gameProcess::loadGameMap() {
    std::ifstream mapInput{gameSettings_};
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

  std::vector<std::pair<std::string, int>> gameProcess::readGameStats() {
    std::ifstream statsStream{kGameStats};
    std::vector<std::pair<std::string, int>> stats;
    stats.reserve(kStatsQuantity);
    if( !statsStream ) {
      throw;// todo
    }

    std::string statsLineKey;
    int statsLineValue;
    for( int i = 0; i < kStatsQuantity || statsStream.eof(); ++i ) {
      statsStream >> statsLineKey >> statsLineValue;
      stats.emplace_back(statsLineKey, statsLineValue);
      if( !statsStream ) {
        throw;// todo
      }
    }

    return stats;
  }

  void gameProcess::showMenu() {
    std::vector<std::pair<std::string, int>> stats{readGameStats()};
    /*std::pair<int, int> nameCoords{screenSize_.startX0 + screenSize_.width / 2,
                                   screenSize_.startY0};*/
    std::pair<std::string, std::pair<int, int>> gameNameField{
            kGameName, std::pair{screenSize_.startX0 + screenSize_.width / 2, screenSize_.startY0}
    };

    for( ; ; ) {
    }
  }


}// namespace gameProcess
