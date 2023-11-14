#pragma once

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include "basic_screen.hpp"

namespace gScreen {
  class gameScreen: public BasicScreen {
  public:
    gameScreen(const BasicScreen& kScreen, const std::string& kSettingsPath);
    virtual ~gameScreen();

    void updateGameStat(const std::string& key, std::string&& value);
    void updateGameStat(const std::string& key, const int);

    void drawObj(const std::vector<std::pair<int, int>>& objectCoords,
                         const std::vector<char>& avatar);
    void deleteObj(const std::vector<std::pair<int, int>>& objectCoords);
    std::vector<std::pair<bool, bool>> fixCollision(const std::vector<std::pair<int, int>>& Route);
    //    bool fixCollision(const std::pair<int, int>& objectCoords, std::pair<int, int>&
    //    objectShift);
    windowSettings GetMapSize() const noexcept;

  protected:
    std::string gameMap_;
    char emptySpace_;
    windowSettings gameMapSize_{}, gameStatsSize_{};
    std::map<std::string, std::pair<std::pair<int, int>, int>> gameStats_;

    void loadGameMap(boost::property_tree::ptree&& kMapSettings);
    void drawGameMap(boost::property_tree::ptree&& kMapSettings);

    void loadGameStats(boost::property_tree::ptree&& kStats);
    void drawGameStats(boost::property_tree::ptree&& kStats);
  };
}// namespace gScreen