#include "game_screen.hpp"

namespace screeen {
  gameScreen::gameScreen(const basicScreen& screen, const std::vector<int>& gameMapSize,
                         const std::vector<int>& gameStatsSize, std::vector<char>&& map,
                         char emptySpace,
                         std::vector<std::pair<std::pair<int, int>, std::string>>&& stats)
      : basicScreen(screen) {
    gameMapSize_ = subWindowSize{
            .width = gameMapSize[0],
            .height = gameMapSize[1],
            .startX = (gameMapSize.size() >= 3) ? gameMapSize[2] : 0,
            .startY = (gameMapSize.size() >= 4) ? gameMapSize[3] : 0,
    };
    gameStatsSize_ = subWindowSize{
            .width = gameStatsSize[0],
            .height = gameStatsSize[1],
            .startX = (gameStatsSize.size() >= 3) ? gameStatsSize[2] : 0,
            .startY = (gameStatsSize.size() >= 4) ? gameStatsSize[3] : 0,
    };
    loadGameMap(std::move(map), emptySpace);
    loadGameStats(std::move(stats));
    drawGameMap();
    drawGameStats();
  }

  gameScreen::~gameScreen() {
    initBaseScreen();
  }

  void gameScreen::loadGameStats(std::vector<std::pair<std::pair<int, int>, std::string>>&& stats) {
    for( auto& statsField: stats ) {
      statsField.first.first += statsField.second.size();
      gameStats_[statsField.second] = statsField.first;
    }
  }

  void gameScreen::drawGameStats() {
    for( const auto& statsField: gameStats_ ) {
      const char *key = statsField.first.data();
      const int x = statsField.second.first - statsField.first.size();
      const int y = statsField.second.second;
      mvwaddstr(window_, y, x, key);
      wrefresh(window_);
    }
  }

  void gameScreen::updateGameStat(const std::string& key, std::string&& value) {
    const int x = gameStats_[key].first;
    const int y = gameStats_[key].second;
    mvwaddstr(window_, y, x, value.data());
    wrefresh(window_);
  }

  void gameScreen::loadGameMap(std::vector<char>&& map, char emptySpace) {
    gameMap_ = std::move(map);
    emptySpace_ = emptySpace;
  }

  void gameScreen::drawMoveGameObj(const std::pair<int, int>& objectCoords,
                                   const std::pair<int, int>& objectShift, const char avatar) {
    mvwaddch(window_, gameMapSize_.startY + objectCoords.second,
             gameMapSize_.startX + objectCoords.first,
             gameMap_[objectCoords.first + objectCoords.second * gameMapSize_.width]);
    mvwaddch(window_, gameMapSize_.startY + objectCoords.second + objectShift.second,
             gameMapSize_.startX + objectCoords.first + objectShift.first, avatar);

    wrefresh(window_);
  }

  void gameScreen::drawGameMap() {
    for( int i = 0; i < gameMap_.size(); ++i ) {
      mvwaddch(window_, (i / gameMapSize_.width) + gameMapSize_.startY,
               (i % gameMapSize_.width) + gameMapSize_.startX, gameMap_[i]);
      wrefresh(window_);
    }
  }

  void gameScreen::deleteGameObj(const std::pair<int, int>& objectCoords) {
    mvwaddch(window_, gameMapSize_.startY + objectCoords.second,
             gameMapSize_.startX + objectCoords.first,
             gameMap_[objectCoords.first + objectCoords.second * gameMapSize_.width]);

    wrefresh(window_);
  }

  bool gameScreen::fixCoordsToMove(const std::pair<int, int>& objectCoords,
                                   std::pair<int, int>& objectShift) {
    if( 0 <= (objectCoords.first + objectShift.first)
        && (objectCoords.first + objectShift.first) < gameMapSize_.width
        && 0 <= (objectCoords.second + objectShift.second)
        && (objectCoords.second + objectShift.second) < gameMapSize_.height
        && gameMap_[(objectCoords.first + objectShift.first)
                    + (objectCoords.second + objectShift.second) * gameMapSize_.width]
                   == emptySpace_ ) {
      return false;
    }
    else {
      if( objectCoords.first + objectShift.first < 0 ) {
        objectShift.first = -objectCoords.first;
      }
      if( objectCoords.first + objectShift.first >= gameMapSize_.width ) {
        objectShift.first = gameMapSize_.width - 1 - objectCoords.first;
      }


      if( objectCoords.second + objectShift.first < 0 ) {
        objectShift.second = -objectCoords.second;
      }
      if( objectCoords.second + objectShift.second >= gameMapSize_.width ) {
        objectShift.second = gameMapSize_.height - 1 - objectCoords.second;
      }

      if( gameMap_[(objectCoords.first + objectShift.first)
                   + (objectCoords.second + objectShift.second) * gameMapSize_.width]
          != emptySpace_ ) {
        objectShift.first = 0;
      }
      if( gameMap_[(objectCoords.first + objectShift.first)
                   + (objectCoords.second + objectShift.second) * gameMapSize_.width]
          == emptySpace_ ) {
        objectShift.second = 0;
      }

      return true;
    }
  }
}// namespace screeen
