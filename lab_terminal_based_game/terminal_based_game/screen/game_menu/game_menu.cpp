#include "game_menu.hpp"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <fstream>

namespace {
  const std::string gkInsertNameField = "insert_name_field";
  const std::string gkInsertNameFieldFields = "fields";
  const std::string gkField = "field";
  const std::string gkFieldWidth = "field_width";
  const std::string gkCursorPos = "cursor_pos";
  const std::string gkCursorPosName = "cursor_pos_name";
  const std::string gkCursorPosScore = "cursor_pos_score";
  const std::string gkWidth = "width";
  const std::string gkHeight = "height";
  const std::string gkGameName = "game_name";
  const std::string gkX0 = "x";
  const std::string gkY0 = "y";
  const std::string gkScoreTable = "score_table";
  const std::string gkScorePath = "score.json";
  const std::string gkPlayerName = "player";
  const std::string gkPlayerScore = "score";
  const std::string gkCcoreTitle = "score";

  const int gkExit = ']';
  const int gkMaxNameLen = 15;// todo hide this inf in json
  const int gkEmptyNameFiller = '_';
}// namespace

namespace gScreen {
  gameMenu::gameMenu(const basicScreen& screen, const std::string& kGameMenuPath,
                     const std::string& scorePath)
      : basicScreen(screen) {
    boost::property_tree::ptree gameMenuConfig;
    boost::property_tree::read_json(kGameMenuPath, gameMenuConfig);

    drawScoreTable(std::move(gameMenuConfig.get_child(gkScoreTable)), scorePath);
    drawGameName(std::move(gameMenuConfig.get_child(gkGameName)));
    drawNameInsertField(std::move(gameMenuConfig.get_child(gkInsertNameField)));
  }

  void gameMenu::drawGameName(boost::property_tree::ptree&& gameName) {
    const int kX0 = gameName.get<int>(gkX0, 0);
    const int kY0 = gameName.get<int>(gkY0, 0);
    const auto name = gameName.get<std::string>(gkGameName);
    mvwaddstr(window_, screenSize_.startY + kY0, screenSize_.startX + kX0, name.data());
    wrefresh(window_);
  }

  void gameMenu::drawNameInsertField(boost::property_tree::ptree&& nameInsertField) {
    playerName_.insert(0, gkMaxNameLen, gkEmptyNameFiller);

    const int kX0 = nameInsertField.get<int>(gkX0, 0);
    const int kY0 = nameInsertField.get<int>(gkY0, 0);
    playerNameInsertPos = {nameInsertField.get_child(gkCursorPos).get<int>(gkX0),
                           nameInsertField.get_child(gkCursorPos).get<int>(gkY0)};
    const boost::property_tree::ptree kFields = nameInsertField.get_child(gkInsertNameFieldFields);
    for( const auto& kField: kFields ) {
      const int kX = kField.second.get<int>(gkX0, 0);
      const int kY = kField.second.get<int>(gkY0, 0);
      const auto name = kField.second.get<std::string>(gkField);
      mvwaddstr(window_, screenSize_.startY + kY0 + kY, screenSize_.startX + kX0 + kX, name.data());
      wrefresh(window_);
    }
    mvwaddstr(window_, screenSize_.startY + playerNameInsertPos.second,
              screenSize_.startX + playerNameInsertPos.first, playerName_.data());
    wrefresh(window_);
  }

  void gameMenu::drawScoreTable(boost::property_tree::ptree&& scoreTable,
                                const std::string& scorePath) {
    const std::pair<int, int> kScoreTableCoords = {screenSize_.startX + scoreTable.get<int>(gkX0),
                                                   screenSize_.startY + scoreTable.get<int>(gkY0)};

    const int kCursorPosNameMaxLen = scoreTable.get_child(gkCursorPosName).get<int>(gkWidth);
    const std::pair<int, int> kPlayerNameCoords = {
            scoreTable.get_child(gkCursorPosName).get<int>(gkX0) + kScoreTableCoords.first,
            scoreTable.get_child(gkCursorPosName).get<int>(gkY0) + kScoreTableCoords.second};

    const int kCursorPosScoreMaxLen = scoreTable.get_child(gkCursorPosScore).get<int>(gkWidth);
    const std::pair<int, int> kPlayerScoreCoords = {
            scoreTable.get_child(gkCursorPosScore).get<int>(gkX0) + kScoreTableCoords.first,
            scoreTable.get_child(gkCursorPosScore).get<int>(gkY0) + kScoreTableCoords.second};

    const int kScoreBorderWidth = scoreTable.get<int>(gkWidth);
    const int kScoreBorderHeight = scoreTable.get<int>(gkHeight);

    boost::property_tree::ptree scores;
    boost::property_tree::read_json(gkScorePath, scores);

    auto scoreTableDraw = scoreTable.get<std::string>(gkScoreTable);
    for( int y = 0; y < kScoreBorderHeight; y++ ) {
      mvwaddnstr(window_, kScoreTableCoords.second + y, kScoreTableCoords.first,
                 scoreTableDraw.data() + kScoreBorderWidth * y, kScoreBorderWidth);
      wrefresh(window_);
    }

    int line = 0;
    for( const auto& field: scores.get_child(gkCcoreTitle) ) {
      std::string score = std::to_string(field.second.get<int>(gkPlayerScore));
      int precision = kCursorPosScoreMaxLen - std::min<int>(kCursorPosScoreMaxLen, score.size());
      score.insert(0, precision, gkEmptyNameFiller);

      mvwaddnstr(window_, kPlayerNameCoords.second + line, kPlayerNameCoords.first,
                 field.second.get<std::string>(gkPlayerName).data(), kCursorPosNameMaxLen);
      mvwaddnstr(window_, kPlayerScoreCoords.second + line, kPlayerScoreCoords.first, score.data(),
                 kCursorPosScoreMaxLen);
      wrefresh(window_);
      ++line;
    }
    wrefresh(window_);
  }

  bool gameMenu::introducePlayerName(const int input) {
    if( input == KEY_ENTER || input == 10 ) {
      playerName_.resize(curNameLen_);
      nameIntroduced_ = true;
    }
    else if( (input == KEY_BACKSPACE || input == KEY_DC || input == 127) ) {
      if( curNameLen_ > 0 ) {
        playerName_[--curNameLen_] = gkEmptyNameFiller;
      }
    }
    else if( curNameLen_ < gkMaxNameLen && input != ERR) {
      playerName_[curNameLen_++] = input;
    }
    mvwaddnstr(window_, screenSize_.startY + playerNameInsertPos.second,
               screenSize_.startX + playerNameInsertPos.first, playerName_.data(),
               playerName_.size());
    wrefresh(window_);

    return nameIntroduced_;
  }

  std::string gameMenu::getPlayerName() {
    return playerName_;
  }

}// namespace gScreen
