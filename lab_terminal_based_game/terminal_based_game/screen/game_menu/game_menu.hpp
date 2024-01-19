#pragma once

#include "basic_screen.hpp"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

namespace gScreen {
  class gameMenu: public BasicScreen {
  public:
    gameMenu(const BasicScreen& screen, const std::string& kGameMenuPath,
             const std::string& scorePath);
    virtual ~gameMenu() = default;

    void drawScoreTable(boost::property_tree::ptree&& scoreTable, const std::string& scorePath);
    void drawGameName(boost::property_tree::ptree&& gameName);
    void drawNameInsertField(boost::property_tree::ptree&& nameInsertField);
    void drawEnemyQuantity(boost::property_tree::ptree&& json);
    int process();
    std::tuple<std::string, std::vector<int>> getIntroducedInf();

  protected:
    windowSettings GameMenuSettings{};

    struct NameIntroduceInf {
      std::pair<int, int> insertPos{};
      std::string name{};
      int len = 0;
    } NameInf_;

    struct EnemyIntroduceInf {
      std::vector<std::pair<int, int>> insertPos{};
      std::vector<std::pair<int, int>> cursorPos{};
      std::vector<int> values{};
      std::vector<int> width;
      std::string cursorAvatar{};
      std::string noCursor{};
      int enemyIndex = 0;
      int cursorRange;
    } EnemyInf_;

    int ApproveIndex_ = 0;

    void introducePlayerName(const int c);
    void introduceEnemy(const int c);
    void shiftEnemyIntroduceCursor(const int kPrevPos);
    void updateEnemyIntroduceStats();
    void getEnemyQuantityChoseInf(const int xIndent, const int yIndent,
                                  boost::property_tree::ptree&& json);
  };
}// namespace gScreen
