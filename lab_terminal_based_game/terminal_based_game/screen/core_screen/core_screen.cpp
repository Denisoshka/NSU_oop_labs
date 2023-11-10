#include "core_screen.hpp"

namespace gScreen {
  coreScreen::coreScreen() {
    initscr();
    getmaxyx(stdscr, terminalSize_.height, terminalSize_.width);
    window_ = newwin(terminalSize_.height, terminalSize_.width, terminalSize_.startY,
                     terminalSize_.startX);
    if( !window_ ) {
      throw;// todo
    }
  }

  coreScreen::~coreScreen() {
    delwin(window_);
    endwin();
  }

  windowSettings coreScreen::getTerminalSize() const noexcept {
    return terminalSize_;
  }

  WINDOW *coreScreen::getWINDOW() const noexcept {
    return window_;
  }
}// namespace gScreen