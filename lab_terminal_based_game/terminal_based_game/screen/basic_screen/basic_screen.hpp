#pragma once

#include "core_screen.hpp"

#include <curses.h>
#include <map>
#include <memory>
#include <vector>

namespace gScreen {
  class basicScreen {
  public:
    basicScreen(const coreScreen& otherScreen);
    basicScreen(const basicScreen& otherScreen);
    virtual ~basicScreen() = default;
    void initBaseScreen();

    int screenInput();

  protected:
    windowSettings terminalSize_{}, screenSize_{};

    WINDOW *window_;
  };
}// namespace gScreen
