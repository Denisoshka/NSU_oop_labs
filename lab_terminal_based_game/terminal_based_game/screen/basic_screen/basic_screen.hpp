#pragma once

#include <curses.h>
#include <map>
#include <memory>
#include <vector>

namespace screeen {
  class basicScreen {
  public:
    basicScreen();
    basicScreen(const basicScreen& otherScreen);
    virtual ~basicScreen();
    void initBaseScreen();

//    WINDOW *getWindow();

    int screenInput();

  protected:
    struct subWindowSize {
      int width, height, startX = 0, startY = 0;
    } terminalSize_{}, screenSize_{};

    WINDOW *window_;
  };




}// namespace screeen
