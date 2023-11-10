#pragma once

#include <curses.h>
#include "core_screen.hpp"

namespace gScreen {
  struct windowSettings {
    int width, height, startX = 0, startY = 0;
  };

  class coreScreen {
  public:
    coreScreen();
    virtual ~coreScreen();
    windowSettings getTerminalSize() const noexcept;
    WINDOW * getWINDOW() const noexcept;
  protected:
    windowSettings terminalSize_{};
    WINDOW *window_;
  };
}// namespace gScreen
