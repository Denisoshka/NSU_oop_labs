#pragma once

#include "basic_screen.hpp"
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

namespace gScreen {
  class gameScreen: basicScreen {
  public:
    gameScreen(const basicScreen& kScreen, const std::string & kSettingsPath);
    virtual ~gameScreen();
    void updateGameStat(const std::string& key, std::string&& value);
    void drawMoveGameObj(const std::pair<int, int>& objectCoords,
                         const std::pair<int, int>& objectShift, const char avatar);
    void deleteGameObj(const std::pair<int, int>& objectCoords);
    bool fixCoordsToMove(const std::pair<int, int>& objectCoords, std::pair<int, int>& objectShift);

  protected:
    void loadGameMap(boost::property_tree::ptree&& kMapSettings);
    void drawGameMap();

    void loadGameStats( boost::property_tree::ptree&& kStats);
    void drawGameStats();

    std::string gameMap_;
    char emptySpace_;
    subWindowSize gameMapSize_{}, gameStatsSize_{};
    std::map<std::string, std::pair<int, int>> gameStats_;
  };
}// namespace gScreen