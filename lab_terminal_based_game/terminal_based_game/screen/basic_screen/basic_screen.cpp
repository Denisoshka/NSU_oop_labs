#include "basic_screen.hpp"
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

namespace {
  const char *gkWidth = "width";
  const char *gkHeight = "height";
  const char *gkX0 = "x";
  const char *gkY0 = "y";
}// namespace

namespace gScreen {

  BasicScreen::BasicScreen(const CoreScreen& otherScreen)
      : window_(otherScreen.getWINDOW())
      , terminalSize_(otherScreen.getTerminalSize())
      , screenSize_(otherScreen.getTerminalSize()) {
    initBaseScreen();
  }

  BasicScreen::BasicScreen(const BasicScreen& otherScreen)
      : window_(otherScreen.window_)
      , terminalSize_(otherScreen.terminalSize_)
      , screenSize_(otherScreen.terminalSize_) {
    initBaseScreen();
  }

  void BasicScreen::initBaseScreen() {
    werase(window_);
    box(window_, 0, 0);
    raw();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    wrefresh(window_);
    wtimeout(window_, 1);

    screenSize_ = terminalSize_;
    ++screenSize_.X0;
    ++screenSize_.Y0;
    screenSize_.w -= 2;
    screenSize_.h -= 2;
  }

  void BasicScreen::centerScreen(const boost::property_tree::ptree& gameScreenConfig) {
    const int screenWidth = gameScreenConfig.get<int>(gkWidth);
    const int screenHeight = gameScreenConfig.get<int>(gkHeight);
    screenSize_.X0 += (screenSize_.w - screenWidth) / 2;
    screenSize_.Y0 += (screenSize_.h - screenHeight) / 2;
  }

  int BasicScreen::input() {
    int input = wgetch(window_);
    flushinp();
    return input;
  }

}// namespace gScreen