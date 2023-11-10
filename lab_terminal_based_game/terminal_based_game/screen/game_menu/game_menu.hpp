#pragma once

#include "basic_screen.hpp"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

namespace gScreen {
  class gameMenu: public basicScreen {
  public:
    gameMenu(const basicScreen& screen, const std::string& kGameMenuPath,
             const std::string& scorePath);
    virtual ~gameMenu() = default;

   /* void drawGameMenu(const std::string& kScorePath);
    void drawGameMenu(const std::vector<std::pair<std::pair<int, int>, std::string>>& stats);
    void updateGameMenu(const std::string_view& key, std::string_view&& value);
    void drawGameMenu(const std::vector<std::pair<std::string, int>>& score);*/
    void drawScoreTable(boost::property_tree::ptree&& scoreTable, const std::string& scorePath);
    void drawGameName(boost::property_tree::ptree&& gameName);
    void drawNameInsertField(boost::property_tree::ptree&& nameInsertField);
    bool introducePlayerName(const int input);
    std::string getPlayerName();

  protected:
    std::pair<int, int> playerNameInsertPos{};
    std::string playerName_{};
    int curNameLen_ = 0;
    bool nameIntroduced_ = false;
  };
}// namespace gScreen
