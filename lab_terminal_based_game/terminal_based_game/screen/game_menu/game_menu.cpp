#include "game_menu.hpp"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <fstream>

namespace {
  const std::string gkInsertNameField = "insert_name_field";
  const std::string gkInsertNameFieldFields = "fields";
  const std::string gkCursorPos = "cursor_pos";
  const std::string gkCursorPosName = "cursor_pos_name";
  const std::string gkCursorPosScore = "cursor_pos_score";
  const std::string gkWidthName = "width";
  const std::string gkHeightName = "height";
  const std::string gkGameName = "game_name";
  const std::string gkX0 = "x";
  const std::string gkY0 = "y";
  const std::string gkScoreTable = "score_table";
  const std::string gkScorePath = "score.json";
  const std::string gkPlayerName = "player";
  const std::string gkPlayerScore = "score";
}// namespace

namespace gScreen {
  gameMenu::gameMenu(const basicScreen& screen, const std::string& kGameMenuPath)
      : basicScreen(screen) {
    boost::property_tree::ptree gameMenuConfig;
    boost::property_tree::read_json(kGameMenuPath, gameMenuConfig);

    drawScoreTable(std::move(gameMenuConfig.get_child(gkScoreTable)), gkScorePath);
    drawGameName(std::move(gameMenuConfig.get_child(gkGameName)));
    drawNameInsertField(std::move(gameMenuConfig.get_child(gkInsertNameField)));
  }

  void gameMenu::drawGameName(boost::property_tree::ptree&& gameName) {
    const int kX0 = gameName.get<int>(gkX0, 0);
    const int kY0 = gameName.get<int>(gkY0, 0);
    const auto name = gameName.get<std::string>(gkGameName);
    mvwaddstr(window_, screenSize_.startY + kY0, screenSize_.startX + kX0, name.data());
  }

  void gameMenu::drawNameInsertField(boost::property_tree::ptree&& nameInsertField) {
    playerNameInsertPos = {nameInsertField.get_child(gkCursorPos).get<int>(gkX0),
                           nameInsertField.get_child(gkCursorPos).get<int>(gkY0)};

    const int kX0 = nameInsertField.get<int>(gkX0, 0);
    const int kY0 = nameInsertField.get<int>(gkY0, 0);

    const boost::property_tree::ptree kFields = nameInsertField.get_child(gkInsertNameFieldFields);
    for( const auto& kField: kFields ) {
      const int kX = kField.second.get<int>(gkX0, 0);
      const int kY = kField.second.get<int>(gkY0, 0);
      const auto name = kField.second.get<std::string>(gkGameName);
      mvwaddstr(window_, screenSize_.startY + kY0 + kY, screenSize_.startX + kX0 + kX, name.data());
    }
  }

  /*void gameMenu::drawScoreBorder(const boost::property_tree::ptree& scoreTable){
  }*/

  void gameMenu::drawScoreTable(boost::property_tree::ptree&& scoreTable,
                                const std::string& scorePath) {
    const int kScoreX0 = screenSize_.startY + scoreTable.get<int>(gkX0);
    const int kScoreY0 = screenSize_.startX + scoreTable.get<int>(gkX0);
    const int kScoreBorderWidth = scoreTable.get<int>(gkWidthName);
    const int kScoreBorderHeight = scoreTable.get<int>(gkHeightName);
    const int kCursorPosX0Name =
            scoreTable.get_child(gkCursorPos).get_child(gkCursorPosName).get<int>(gkX0);
    const int kCursorPosX0Score =
            scoreTable.get_child(gkCursorPos).get_child(gkCursorPosScore).get<int>(gkX0);
    const int kCursorPosNameMaxLen =
            scoreTable.get_child(gkCursorPos).get_child(gkCursorPosName).get<int>(gkWidthName);
    const int kCursorPosScoreMaxLen =
            scoreTable.get_child(gkCursorPos).get_child(gkCursorPosScore).get<int>(gkWidthName);
    boost::property_tree::ptree scores;
    boost::property_tree::read_json(gkScorePath, scores);
    const std::pair<int, int> kCursorPos{scoreTable.get_child(gkCursorPos).get<int>(gkX0),
                                         scoreTable.get_child(gkCursorPos).get<int>(gkY0)};


    auto scoreTableDraw = scoreTable.get<std::string>(gkScoreTable);
    for( int y = 0; y < kScoreBorderHeight; y++ ) {
      mvwaddnstr(window_, kScoreY0 + y, kScoreX0, scoreTableDraw.data() + kScoreBorderWidth * y,
                 kScoreBorderWidth);
    }
    
    for( const auto& field: scores ) {
      const int y = kScoreY0 + kCursorPos.second;
      const int x = kScoreX0 + kCursorPos.first;
      std::string score = std::to_string(field.second.get<int>(gkPlayerScore));
      int precision = kCursorPosScoreMaxLen - std::min<int>(kCursorPosScoreMaxLen, score.size());
      score.insert('_', precision, kCursorPosScoreMaxLen);

      mvwaddnstr(window_, y, x + kCursorPosX0Name,
                 field.second.get<std::string>(gkPlayerName).data(), kCursorPosNameMaxLen);
      mvwaddnstr(window_, y, x + kCursorPosX0Score, score.data(), kCursorPosScoreMaxLen);
    }
  }

}// namespace gScreen