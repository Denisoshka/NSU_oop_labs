#include "core_screen.hpp"

namespace gScreen {
  CoreScreen::CoreScreen() {
    initscr();
    getmaxyx(stdscr, terminalSize_.h, terminalSize_.w);
    window_ = newwin(terminalSize_.h, terminalSize_.w, terminalSize_.Y0,
                     terminalSize_.X0);
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