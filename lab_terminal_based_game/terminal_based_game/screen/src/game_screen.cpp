#include "game_screen.hpp"

#include <fstream>
#include <iostream>

namespace gameScreen {
  defaultScreen::defaultScreen(const std::vector<int>& gameMapSize,
                               const std::vector<int>& gameStatsSize)
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

  void defaultScreen::loadGameScreenInf(
          const std::vector<int>& gameMapSize, const std::vector<int>& gameStatsSize,
          std::vector<char>&& map,
          std::vector<std::pair<std::pair<int, int>, std::string_view>>&& stats, char emptySpace) {
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
  }

  void defaultScreen::initScreen() {
    initscr();
    getmaxyx(stdscr, terminalSize_.height, terminalSize_.width);
    window_ = newwin(terminalSize_.height, terminalSize_.width, 0, 0);
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

  void defaultScreen::loadGameStats(
          std::vector<std::pair<std::pair<int, int>, std::string_view>>&& stats) {
    for( auto& statsField: stats ) {
      statsField.first.first += statsField.second.size();
      gameStats_[statsField.second] = statsField.first;
    }
  }

  void defaultScreen::drawGameStats() {
    for( const auto& statsField: gameStats_ ) {
      const char *key = statsField.first.data();
      const int x = statsField.second.first - statsField.first.size();
      const int y = statsField.second.second;
      mvwaddstr(window_, y, x, key);
      wrefresh(window_);
    }
  }

  void defaultScreen::updateGameStat(const std::string_view& key, std::string_view&& value) {
    const int x = gameStats_[key].first;
    const int y = gameStats_[key].second;
    mvwaddstr(window_, y, x, value.data());
    wrefresh(window_);
  }

  void defaultScreen::loadGameMap(std::vector<char>&& map, char wall) {

  }

  void defaultScreen::drawMoveGameObj(const std::pair<int, int>& objectCoords,
                                      const std::pair<int, int>& objectShift, const char avatar) {
    mvwaddch(window_, gameMapSize_.startY + objectCoords.second,
             gameMapSize_.startX + objectCoords.first,
             gameMap_[objectCoords.first + objectCoords.second * gameMapSize_.width]);
    mvwaddch(window_, gameMapSize_.startY + objectCoords.second + objectShift.second,
             gameMapSize_.startX + objectCoords.first + objectShift.first, avatar);

    wrefresh(window_);
  }

  void defaultScreen::drawGameMap() {
    for( int i = 0; i < gameMap_.size(); ++i ) {
      mvwaddch(window_, (i / gameMapSize_.width) + gameMapSize_.startY,
               (i % gameMapSize_.width) + gameMapSize_.startX, gameMap_[i]);
      wrefresh(window_);
    }
  }

  bool defaultScreen::fixCoordsToMove(const std::pair<int, int>& objectCoords,
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

  defaultScreen::~defaultScreen() {
    delwin(window_);
    endwin();
  }

  WINDOW *defaultScreen::getWindow() {
    return window_;
  }

  void defaultScreen::deleteGameObj(const std::pair<int, int>& objectCoords) {
    mvwaddch(window_, gameMapSize_.startY + objectCoords.second,
             gameMapSize_.startX + objectCoords.first,
             gameMap_[objectCoords.first + objectCoords.second * gameMapSize_.width]);

    wrefresh(window_);
  }

  defaultScreen& defaultScreen::operator=(defaultScreen&& otherScreen) {
    if( this == &otherScreen ) {
      return *this;
    }
    gameMapSize_ = otherScreen.gameMapSize_;
    gameStatsSize_ = otherScreen.gameStatsSize_;
    gameStats_ = std::move(otherScreen.gameStats_);
    gameMap_ = std::move(otherScreen.gameMap_);
    emptySpace_ = otherScreen.emptySpace_;
    if( window_ ) {
      delwin(window_);
    }
    window_ = otherScreen.window_;
    otherScreen.window_ = nullptr;

    terminalSize_ = std::move(otherScreen.terminalSize_);
    return *this;
  }

  defaultScreen::defaultScreen() {
    initscr();
    getmaxyx(stdscr, terminalSize_.height, terminalSize_.width);
    window_ = newwin(terminalSize_.height, terminalSize_.width, terminalSize_.startY,
                     terminalSize_.startX);
    menuSize_ = terminalSize_;
    ++menuSize_.startX;
    ++menuSize_.startY;
    if( !window_ ) {
      throw;// todo
    }
    box(window_, 0, 0);
    curs_set(FALSE);
    keypad(window_, TRUE);
    nodelay(window_, TRUE);
  }

  void defaultScreen::drawGameMenu(
          const std::vector<std::pair<std::pair<int, int>, std::string>>& stats) {
    void initGameMenu();
    for( auto& statField: stats ) {
      const char *key = statField.second.data();
      const int x = statField.first.first;
      const int y = statField.first.second;
      mvwaddstr(window_, y, x, key);
      wrefresh(window_);
    }
  }

  void defaultScreen::initGameMenu() {
    curs_set(FALSE);
    keypad(window_, TRUE);
    noecho();
    noraw();
    werase(window_);
    box(window_, 0, 0);
    wrefresh(window_);
  }

  int defaultScreen::screenInput() {
    return wgetch(window_);
  }

  /*  void defaultScreen::drawGameMenu(const std::string& kStatsPath) {
    std::ifstream playersStats {kStatsPath};
  }
  void defaultScreen::drawGameMenu(const std::vector<std::pair<std::pair<int, int>,
  std::string_view>>& stats){ for(auto& statField : stats){ const char *key =
  statsField.first.data();

    }
  }

  menuScreen::menuScreen(const defaultScreen& screen) {
    window_ = screen.window_;
  }*/
}// namespace gameScreen