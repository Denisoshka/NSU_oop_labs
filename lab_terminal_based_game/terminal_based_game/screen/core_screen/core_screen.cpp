#include "core_screen.hpp"

namespace gScreen {
  CoreScreen::CoreScreen() {
    initscr();
    getmaxyx(stdscr, terminalSize_.height, terminalSize_.width);
    window_ = newwin(terminalSize_.height, terminalSize_.width, terminalSize_.startY,
                     terminalSize_.startX);
    if( !window_ ) {
      throw;// todo
    }
  }

  CoreScreen::~CoreScreen() {
    delwin(window_);
    endwin();
  }

  windowSettings CoreScreen::getTerminalSize() const noexcept {
    return terminalSize_;
  }

  WINDOW *CoreScreen::getWINDOW() const noexcept {
    return window_;
  }
}// namespace gScreen