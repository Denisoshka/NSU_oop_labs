#pragma once

#include "core_screen.hpp"

#include <curses.h>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <map>
#include <memory>
#include <vector>

namespace gScreen {
  class BasicScreen {
  public:
    BasicScreen(const CoreScreen& otherScreen);
    BasicScreen(const BasicScreen& otherScreen);
    virtual ~BasicScreen() = default;
    void initBaseScreen();

    int screenInput();

  protected:
    windowSettings terminalSize_{}, screenSize_{};

    WINDOW *window_;
    void centerScreen(const boost::property_tree::ptree& gameScreenConfig);
  };
}// namespace gScreen
