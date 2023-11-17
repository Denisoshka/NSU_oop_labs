#pragma once

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include "basic_screen.hpp"

namespace gScreen {
  class BasicGameScreen: public BasicScreen {
  public:
  protected:
  };

  class GameScreen: public BasicScreen {
  public:
    GameScreen(const BasicScreen& kScreen, const std::string& kSettingsPath,
               const std::string& kGameMap, const windowSettings& kGameMapSize);
    virtual ~GameScreen();

    void updateGameStat(const std::string& key, std::string&& value);
    void updateGameStat(const std::string& key, const int);
    void updateMapSettings();

    void drawObj(const std::vector<std::pair<int, int>>& objectCoords,
                 const std::string avatar);
    void deleteObj(const std::vector<std::pair<int, int>>& objectCoords);
    void drawGameMap();

    windowSettings getMapSize() const noexcept;

  protected:
    const std::string& gameMap_{};
    const windowSettings& gameMapSettings_{};

    windowSettings gameMapIndentSettings_{};
    windowSettings gameMapScreenSettings_{};

//    windowSettings gameStatsIndentSettings_{};
    windowSettings gameStatsScreenSettings_{};

    std::map<std::string, std::pair<std::pair<int, int>, int>> gameStats_;
    void loadMapScreenSettings(boost::property_tree::ptree&& settings);
    void loadStatsScreenSettings(boost::property_tree::ptree&& settings);

    void drawGameStats();
  };
}// namespace gScreen