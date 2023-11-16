#pragma once

#include <curses.h>
#include "core_screen.hpp"

namespace gScreen {
  struct windowSettings {
    int w, h, X0 = 0, Y0 = 0;
  };

  class CoreScreen {
  public:
    CoreScreen();
    virtual ~CoreScreen();
    windowSettings getTerminalSize() const noexcept;
    WINDOW *getWINDOW() const noexcept;

  protected:
    windowSettings terminalSize_{};
    WINDOW *window_;
  };
}// namespace gScreen
