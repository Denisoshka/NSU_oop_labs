#include "basic_screen.hpp"

#include <fstream>
#include <iostream>

namespace screeen {
  /*basic_screen::basic_screen(const std::vector<int>& gameMapSize,
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
*/
  /* void basic_screen::loadGameScreenInf(
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
   }*/

  basicScreen::basicScreen(const basicScreen& otherScreen)
      : window_(otherScreen.window_)
      , terminalSize_(otherScreen.terminalSize_)
      , screenSize_(otherScreen.screenSize_) {
    initBaseScreen();
  }

  /*
    basic_screen& basic_screen::operator=(const basic_screen& otherScreen) {
      if( this == &otherScreen ) {
        return *this;
      }

      delwin(window_);
      window_ = otherScreen.window_;
      terminalSize_ = otherScreen.terminalSize_;
    }*/

  basicScreen::basicScreen() {
    initscr();
    getmaxyx(stdscr, terminalSize_.height, terminalSize_.width);
    window_ = newwin(terminalSize_.height, terminalSize_.width, terminalSize_.startY,
                     terminalSize_.startX);
    if( !window_ ) {
      throw;// todo
    }

    initBaseScreen();
  }

  void basicScreen::initBaseScreen() {
    werase(window_);
    box(window_, 0, 0);
    raw();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    wrefresh(window_);

    screenSize_ = terminalSize_;
    ++screenSize_.startX;
    ++screenSize_.startY;
    screenSize_.width -= 2;
    screenSize_.height -= 2;
  }

  basicScreen::~basicScreen() {
    delwin(window_);
    endwin();
  }

  /* WINDOW *basic_screen::getWindow() {
     return window_;
   }*/

  /*
    basic_screen& basic_screen::operator=(basic_screen&& otherScreen) {
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
  */

  int basicScreen::screenInput() {
    int input = wgetch(window_);
    flushinp();
    return input;
  }

  /*  void basic_screen::drawGameMenu(const std::string& kStatsPath) {
    std::ifstream playersStats {kStatsPath};
  }
  void basic_screen::drawGameMenu(const std::vector<std::pair<std::pair<int, int>,
  std::string_view>>& stats){ for(auto& statField : stats){ const char *key =
  statsField.first.data();

    }
  }

  menuScreen::menuScreen(const basic_screen& basic_screen) {
    window_ = basic_screen.window_;
  }*/
}// namespace screeen