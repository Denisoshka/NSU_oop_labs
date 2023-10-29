#include "game_screen.hpp"

#include <fstream>
#include <iostream>

namespace gameScreen {
  gameScreen::gameScreen(const std::vector<int>& gameMapSize, const std::vector<int>& gameStatsSize)
      : gameMapSize_(subWindowSize{
              .width = gameMapSize[0],
              .height = gameMapSize[1],
              .startX = (gameMapSize.size() >= 3) ? gameMapSize[2] : 0,
              .startY = (gameMapSize.size() >= 4) ? gameMapSize[3] : 0,
      })
      , gameStatsSize_(subWindowSize{
                .width = gameStatsSize[0],
                .height = gameStatsSize[1],
                .startX = (gameStatsSize.size() >= 3) ? gameStatsSize[2] : 0,
                .startY = (gameStatsSize.size() >= 4) ? gameStatsSize[3] : 0,
        }) {
    window_ = nullptr;
    initScreen();
  }

  void gameScreen::initScreen() {
    initscr();
    getmaxyx(stdscr, terminalSize_.second, terminalSize_.first);
    window_ = newwin(terminalSize_.second, terminalSize_.first, 0, 0);
    if( !window_ ) {
      endwin();
      throw std::runtime_error("!sWin");
    }

    raw();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
  }

  void gameScreen::loadGameStats(
          std::vector<std::pair<std::pair<int, int>, std::string>>&& kStats) {
  }

  void gameScreen::drawGameStats() {
  }

  void gameScreen::fixGameStats(std::vector<std::pair<std::pair<int, int>, int>>& kStats) {
  }

  void gameScreen::loadGameMap(std::vector<char>&& map, char wall) {
    map_ = std::move(map);
    emptySpace_ = wall;
  }

  void gameScreen::drawGameObj(const std::pair<int, int>& objectCoords,
                               const std::pair<int, int>& objectShift, const char avatar) {
    mvwaddch(window_, gameMapSize_.startY + objectCoords.second,
             gameMapSize_.startX + objectCoords.first,
             map_[objectCoords.first + objectCoords.second * gameMapSize_.width]);
    mvwaddch(window_, gameMapSize_.startY + objectCoords.second + objectShift.second,
             gameMapSize_.startX + objectCoords.first + objectShift.first, avatar);

    mvwaddch(window_, objectCoords.second + objectShift.second,
             objectCoords.first + objectShift.first, avatar);

    wrefresh(window_);
  }

  void gameScreen::drawGameMap() {
    for( int i = 0; i < map_.size(); ++i ) {
      mvwaddch(window_, (i / gameMapSize_.width) + gameMapSize_.startY,
               (i % gameMapSize_.width) + gameMapSize_.startX, map_[i]);
      wrefresh(window_);
    }
  }

  bool gameScreen::fixCoordsToMove(const std::pair<int, int>& objectCoords,
                                   std::pair<int, int>& objectShift) {
    if( 0 <= (objectCoords.first + objectShift.first)
        && (objectCoords.first + objectShift.first) < gameMapSize_.width
        && 0 <= (objectCoords.second + objectShift.second)
        && (objectCoords.second + objectShift.second) < gameMapSize_.height
        && map_[(objectCoords.first + objectShift.first)
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

      if( map_[(objectCoords.first + objectShift.first)
               + (objectCoords.second + objectShift.second) * gameMapSize_.width]
          != emptySpace_ ) {
        objectShift.first = 0;
      }
      if( map_[(objectCoords.first + objectShift.first)
               + (objectCoords.second + objectShift.second) * gameMapSize_.width]
          == emptySpace_ ) {
        objectShift.second = 0;
      }

      return true;
    }
  }

  gameScreen::~gameScreen() {
    delwin(window_);
    endwin();
  }

  WINDOW *gameScreen::getWindow() {
    return window_;
  }


}// namespace gameScreen