#include "process.hpp"
#include "game_obj.hpp"
#include "game_screen.hpp"
#include "shifting_object.hpp"

#include <curses.h>// todo убрать нахуй
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


  void gameProcess::initGameEnvironment() {
    myWeapons.reserve(minimalAmmoQuantity);
    enemyWeapons.reserve(minimalAmmoQuantity);

    std::pair playerCords1 = std::pair{mapSize_.width / 2, mapSize_.height - 2};
    std::pair playerCords2 = playerCords1;
    player = gameObj::Player{gameObj::ekObjUp, std::move(playerCords1)};
    for( int i = 0; i < kEnemyQuantity; ++i ) {
      gameObjects.push_back(std::make_shared<gameObj::Enemy>(gameObj::ObjDirection::ekOBJDown,
                                                             std::pair{mapSize_.width / 2, 2}));
    }
    mainScreen_.drawMoveGameObj(playerCords2, std::pair{0, 0}, player.avatar());
  }

  void gameProcess::updateGameProcess() {
    for( const auto& a: gameObjects ) {
      std::pair desiredShift = a->desiredShift();
      std::pair objectCoords = a->getCoords();

      auto action = a->action('0');
      if( action ) {
        enemyWeapons.push_back(std::move(action));
      }
      mainScreen_.fixCoordsToMove(objectCoords, desiredShift);
      a->makeShift(desiredShift);
      mainScreen_.drawMoveGameObj(objectCoords, desiredShift, a->avatar());
    }
  }

  gameProcessConstants gameProcess::updateEnemyWeapons() {
    for( auto weapon = enemyWeapons.begin(); weapon != enemyWeapons.end(); ) {
      std::pair desiredShift{(*weapon)->desiredShift()};

      bool flag = mainScreen_.fixCoordsToMove((*weapon)->getCoords(), desiredShift);
      (*weapon)->makeShift(desiredShift);
      mainScreen_.drawMoveGameObj((*weapon)->getCoords(), desiredShift, (*weapon)->avatar());

      if( flag || (*weapon)->getCoords() == player.getCoords() ) {
        mainScreen_.deleteGameObj((*weapon)->getCoords());
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

      bool flag = mainScreen_.fixCoordsToMove(objectCoords, desiredShift);
      (*weapon)->makeShift(desiredShift);
      mainScreen_.drawMoveGameObj(objectCoords, desiredShift, (*weapon)->avatar());

      for( auto object = gameObjects.begin(); object != gameObjects.end(); ) {
        if( (*object)->getCoords() == (*weapon)->getCoords() ) {
          flag = true;
          mainScreen_.deleteGameObj((*object)->getCoords());
          gameObjects.erase(object);
        }
        else {
          ++object;
        }
      }

      if( flag ) {
        mainScreen_.deleteGameObj((*weapon)->getCoords());
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

    mainScreen_.fixCoordsToMove(objectCoords, desiredShift);
    player.makeShift(desiredShift);
    mainScreen_.drawMoveGameObj(objectCoords, desiredShift, player.avatar());

    std::string ammoQuantity = std::to_string(player.getAmmoQuantity());
    ammoQuantity.insert(0, 3 - ammoQuantity.length(), '0');
    mainScreen_.updateGameStat(bulletsField, std::move(ammoQuantity));
  }

  void gameProcess::fillGameMenu(
          std::vector<std::pair<std::pair<int, int>, std::string>>& gameMenu,
          const std::string& kScorePath) {
    std::ifstream ScoreStream{kBasicScorePath};
    if( !ScoreStream ) {
      throw;// todo
    }

    gameMenu.emplace_back(std::pair{
            std::pair{terminalSize_.width / 2 + terminalSize_.startX0 - kDefPlayerNameField.size(),
                      terminalSize_.startY0 + 2},
            kDefPlayerNameField + kDefPlayerName
    });
    std::string scoreField{};
    for( int i = 4; i < kScoreFieldsQuantity && !ScoreStream.eof(); ++i ) {
      if( getline(ScoreStream, scoreField).fail() ) {
        throw;
      }
      gameMenu.emplace_back(std::pair{terminalSize_.startX0 + 2, terminalSize_.startY0 + i},
                         std::move(scoreField));
    }
  }

  int gameProcess::process() {
    while( mainScreen_.screenInput() != kFinishGame ) {
      int input;
//      bool exitFlag{false};
      std::vector<std::pair<std::pair<int, int>, std::string>> menu;
      fillGameMenu(menu, kBasicScorePath);
      mainScreen_.drawGameMenu(menu);

      while( (input = mainScreen_.screenInput()) == ERR ) {
      }

      if( input == 'q' ) {
        break;
      }

      startGame();
    }

    return 0;
  }

  void gameProcess::initGameScreen(
          std::vector<std::pair<std::pair<int, int>, std::string_view>>&& stats) {
    std::vector<char> gameMap;
    this->loadGameMap(gameMap);
    // todo здесь ли грузить карту?
    std::vector<int> mapSize{mapSize_.width, mapSize_.height, kBasicXIndent, kBasicYIndent};
    std::vector<int> statsSize{mapSize_.width, 6, 0, mapSize_.height};
    mainScreen_.loadGameScreenInf(mapSize, statsSize, std::move(gameMap), std::move(stats),
                                  kFreeSpace);
    mainScreen_.drawGameScreen();
  }

  int gameProcess::startGame() {
    std::vector<std::pair<std::pair<int, int>, std::string_view>> stats = {
            {{0, mapSize_.height + 1}, bulletsField     },
            {{0, mapSize_.height + 3}, kElapsedTimeField}
    };
    initGameScreen(std::move(stats));
    srandom(time(nullptr));
    initGameEnvironment();
    auto startGameTime = std::chrono::steady_clock::now();
    int input;
    while( (input = mainScreen_.screenInput()) != kFinishGame ) {
      if( input != ERR ) {
        updatePlayer(input);
      }

      if( gameObjects.empty() ) {
        auto gameEndTime = std::chrono::steady_clock::now();
        startGameTime += gameEndTime - startGameTime;
        break;
      }

      updateGameProcess();
      updateMyWeapons();
      updateEnemyWeapons();
      auto end = std::chrono::steady_clock::now();
      auto seconds = std::to_string(std::chrono::duration_cast<std::chrono::seconds>(end - startGameTime).count());
      mainScreen_.updateGameStat(kElapsedTimeField, std::move(seconds));
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return 0;
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

}// namespace gameProcess
