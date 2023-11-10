#include "basic_screen.hpp"

namespace gScreen {

  basicScreen::basicScreen(const coreScreen& otherScreen)
      : window_(otherScreen.getWINDOW())
      , terminalSize_(otherScreen.getTerminalSize())
      , screenSize_(otherScreen.getTerminalSize()) {
    initBaseScreen();
  }

  basicScreen::basicScreen(const basicScreen& otherScreen)
      : window_(otherScreen.window_)
      , terminalSize_(otherScreen.terminalSize_)
      , screenSize_(otherScreen.terminalSize_) {
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
    wtimeout(window_, 10);

    screenSize_ = terminalSize_;
    ++screenSize_.startX;
    ++screenSize_.startY;
    screenSize_.width -= 2;
    screenSize_.height -= 2;
  }

  int basicScreen::screenInput() {
    int input = wgetch(window_);
    flushinp();
    return input;
  }

}// namespace gScreen