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
  const char kFreeSpace{' '};
  const char kFinishGame{']'};
  const std::string_view bulletsField{"bullets quantity: "};
  const std::string_view kElapsedTimeField{"elapsed time: "};
  const std::string_view kPlayerLives{"life rest :"};
  const int kEnemyQuantity{10};
  const int kMenuFieldsQuantity{12};
  const int kScoreFieldsQuantity{10};
  const int kBasicXIndent = 1;
  const int kBasicYIndent = 1;
  const std::string kDefPlayerNameField{"player name: "};

  const std::string kDefPlayerName{"obezyana"};
  const std::string kGameStats{"game_stats.txt"};
  const std::string kGameName{"Nigga sweg"};
  const std::string kBasicScorePath{"score.txt"};
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

      bool flag = screen_.fixCoordsToMove((*weapon)->getCoords(), desiredShift);
      (*weapon)->makeShift(desiredShift);
      screen_.drawMoveGameObj((*weapon)->getCoords(), desiredShift, (*weapon)->avatar());

      if( flag || (*weapon)->getCoords() == player.getCoords() ) {
        screen_.deleteGameObj((*weapon)->getCoords());
        if( (*weapon)->getCoords() == player.getCoords() ) {
          --playerLives;
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
          ++object;
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

  std::vector<std::pair<std::pair<int, int>, std::string>> gameProcess::fillGameMenu(
          std::vector<std::pair<std::pair<int, int>, std::string>>& score,
          const std::string& kScorePath) {
    std::ifstream ScoreStream{kBasicScorePath};
    if( !ScoreStream ) {
      throw;// todo
    }

    score.emplace_back(std::pair{
            std::pair{terminalSize_.width / 2 + terminalSize_.startX0 - kDefPlayerNameField.size(),
                      terminalSize_.startY0 + 2},
            kDefPlayerNameField + kDefPlayerName
    });
    std::string scoreField{};
    for( int i = 4; i < kScoreFieldsQuantity && !ScoreStream.eof(); ++i ) {
      if( getline(ScoreStream, scoreField).fail() ) {
        throw;
      }
      score.emplace_back(std::pair{terminalSize_.startX0 + 2, terminalSize_.startY0 + i},
                         std::move(scoreField));
    }
  }

  int gameProcess::process() {
    while( mainScreen_.screenInput() != kFinishGame ) {
      int input;
      bool exitFlag{false};
      std::vector<std::pair<std::pair<int, int>, std::string>> menu;
      fillGameMenu(menu, kBasicScorePath);
      mainScreen_.drawGameMenu(menu);

      while( (input = mainScreen_.screenInput()) == ERR ) {
      }
      if( input == 'q' ) {
        //        exitFlag = true;
        break;
      }

      startGame();
    }


    return 0;
  }

  void gameProcess::initGameScreen(
          std::vector<std::pair<std::pair<int, int>, std::string_view>>&& stats) {
    std::vector<int> mapSize{mapSize_.width, mapSize_.height, kBasicXIndent, kBasicYIndent};
    std::vector<int> statsSize{mapSize_.width, 6, 0, mapSize_.height};
    std::vector<char> gameMap;
    this->loadGameMap(gameMap);

    mainScreen_.loadGameMap(std::move(gameMap), kFreeSpace);
    mainScreen_.drawGameMap();
    mainScreen_.loadGameStats(std::move(stats));
    mainScreen_.drawGameStats();
  }

  int gameProcess::startGame() {

    mainScreen_.loadGameMap(std::move(gameMap_), kFreeSpace);
  }

  int gameProcess::startGame() {

    srandom(time(nullptr));
    std::vector<std::pair<std::pair<int, int>, std::string_view>> stats = {
            {{0, mapSize_.height + 1}, bulletsField     },
            {{0, mapSize_.height + 3}, kElapsedTimeField}
    };


    bool exitFlag{false};
    while( true ) {
      initGameScreen(std::move(stats));
      initGameProcessEnvironment();
      auto startGameTime = std::chrono::steady_clock::now();
      while( true ) {
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
  }

  gameProcess::gameProcess::gameProcess(std::string&& gameSettings)
      : gameSettings_(std::move(gameSettings)) {
  }

  void gameProcess::gameProcess::loadGameMap(std::vector<char>& map) {
    std::ifstream mapInput{gameSettings_};
    if( !mapInput.is_open() ) {
      // todo
    }
    mapInput >> mapSize_.width >> mapSize_.height;
    map.resize(mapSize_.width * mapSize_.height);
    char skipNextLineSymbol;
    mapInput.read(&skipNextLineSymbol, sizeof(skipNextLineSymbol));
    if( skipNextLineSymbol != '\n' ) {
      throw std::exception();//        todo
    }
    for( int row = 0; row < mapSize_.height; ++row ) {
      mapInput.read(reinterpret_cast<char *>(map.data() + row * mapSize_.width),
                    mapSize_.width * sizeof(*map.data()));
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

    for( ;; ) {
    }
  }


}// namespace gameProcess
