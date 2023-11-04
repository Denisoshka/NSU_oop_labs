#include "game_menu.hpp"
#include <fstream>

namespace {
  const char scoreSeparator{'|'};
  const int maxStatsQuantity{10};
}// namespace

namespace screeen {
  gameMenu::gameMenu(const basicScreen& screen, const std::vector<int>& gameMenuSize)
      : basicScreen(screen)
      , gameScoreSize_(screenSize_) {
    //    gameScoreSize_
  }

  void gameMenu::drawScoreTable(const std::vector<std::pair<std::string, int>>& score) {
    const int borderSize = 16;
    const int scoreHigh = 10;
    const int maxNameLen = 10;
    const int wBorderStart = screenSize_.startY + 1;
    const int hBorderStart = screenSize_.startX + 1;
    const char tooLongNamePass = '_';

    mvwhline(window_, screenSize_.startY, hBorderStart, '-', borderSize);
    mvwhline(window_, wBorderStart + scoreHigh, hBorderStart, '-', borderSize);
    mvwvline(window_, wBorderStart, screenSize_.startX, '|', scoreHigh);
    mvwvline(window_, wBorderStart, hBorderStart + borderSize, '|', scoreHigh);

    int scoreLineIndex = 0;
    for( const auto& scoreLine: score ) {
      mvwaddnstr(window_, wBorderStart + scoreLineIndex, hBorderStart, scoreLine.first.data(),
                 maxNameLen);
      if( scoreLine.first.size() > maxNameLen ) {
        mvwaddch(window_, wBorderStart + scoreLineIndex, wBorderStart + maxNameLen + 1,
                 tooLongNamePass);
      }


    }
  }

  void gameMenu::drawGameMenu(const std::vector<std::pair<std::string, int>>& score) {
    //    std::ifstream scoreStream{kScorePath};
    //    std::pair<std::string, int> statLine;
    //    for( int stat = 0; !scoreStream.eof() && stat < maxStatsQuantity; ++stat ) {
    //      scoreStream >> statLine.first >> statLine.second;
    //      if( !scoreStream ) {
    //        throw std::exception();
    //      }
    //    }
    drawScoreTable();
  }

  void gameMenu::drawGameMenu(const std::vector<std::string>& score) {
    for( auto& statField: score ) {
      const char *key = statField.second;
      mvwaddstr(window_, y, x, key);
      wrefresh(window_);
    }
  }

  /*  void gameMenu::initGameMenu() {
      curs_set(FALSE);
      keypad(window_, TRUE);
      noecho();
      noraw();
      werase(window_);
      box(window_, 0, 0);
      wrefresh(window_);
    }*/


}// namespace screeen