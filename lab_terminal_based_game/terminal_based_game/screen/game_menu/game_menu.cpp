#include "game_menu.hpp"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <fstream>

namespace {
  const std::string gkInsertNameField = "insert_name_field";
  const std::string gkInsertNameFieldFields = "fields";
  const std::string gkField = "field";
  const std::string gkFieldWidth = "field_width";
  const std::string gkCursor = "cursor";
  const std::string gkCursorPosName = "cursor_pos_name";
  const std::string gkCursorPosScore = "cursor_pos_score";
  const std::string gkWidth = "width";
  const std::string gkHeight = "height";
  const std::string gkGameName = "game_name";
  const std::string gkX0 = "x";
  const std::string gkY0 = "y";
  const std::string gkScoreTable = "score_table";
  const std::string gkScoreTableRowsLimit = "rows_quantity";
  const std::string gkScorePath = "score.json";
  const std::string gkPlayerName = "player";
  const std::string gkPlayerScore = "score";
  const std::string gkScoreTitle = "score";
  const std::string gkScoreFields = "fields";

  const std::string gkEnemyCursorInf = "cursor_inf";
  const std::string gkEnemyCursorSettings = "cursor";
  const std::string gkEnemyCursorInsert = "insert_field";
  const std::string gkEnemyChoseFields = "fields";
  const std::string gkCursorAvatarPos = "cursor";
  const std::string gkCursorAvatarInsert = "insert_field";
  const std::string gkCursorAvatar = "cursor_avatar";
  const std::string gkEmptyCursorAvatar = "empty_cursor";
  const std::string gkEnemyQuantityInsertField = "enemy_quantity_field";
  const std::string gkEnemy1 = "enemy_1";
  const std::string gkEnemy2 = "enemy_2";
  const std::string gkEnemy3 = "enemy_3";
  const std::string gkCursorQuantity = "cursor_quantity";

  const int gkExit = ']';
  const int gkCursorUp = 'w';
  const int gkCursorDown = 's';
  const int gkStatsUp = 'd';
  const int gkStatsDown = 'a';
  const int gkMaxNameLen = 15;// todo maybe hide this inf in json
  const int gkEmptyNameFiller = '_';
  const int gkEmptyEnemyCursorFiller = ' ';
  const int gkApprovesQuantity = 2;
  const int gkEnemyQuantityNotInstalled = -1;

  enum ApproveIndexes : int {
    ekName,
    ekEnemies,
    ekFinish,
  };
}// namespace

namespace gScreen {
  gameMenu::gameMenu(const BasicScreen& screen, const std::string& kGameMenuPath,
                     const std::string& scorePath)
      : BasicScreen(screen) {
    boost::property_tree::ptree gameMenuConfig;
    boost::property_tree::read_json(kGameMenuPath, gameMenuConfig);
    centerScreen(gameMenuConfig);
    drawScoreTable(std::move(gameMenuConfig.get_child(gkScoreTable)), scorePath);
    drawGameName(std::move(gameMenuConfig.get_child(gkGameName)));
    drawNameInsertField(std::move(gameMenuConfig.get_child(gkInsertNameField)));
    drawEnemyQuantity(std::move(gameMenuConfig.get_child(gkEnemyQuantityInsertField)));
  }

  void gameMenu::drawGameName(boost::property_tree::ptree&& gameName) {
    const int kX0 = gameName.get<int>(gkX0, 0);
    const int kY0 = gameName.get<int>(gkY0, 0);
    const auto name = gameName.get<std::string>(gkGameName);
    mvwaddstr(window_, screenSize_.Y0 + kY0, screenSize_.X0 + kX0, name.data());
    wrefresh(window_);
  }

  void gameMenu::drawNameInsertField(boost::property_tree::ptree&& nameInsertField) {
    NameInf_.name.insert(0, gkMaxNameLen, gkEmptyNameFiller);

    const int kX0 = nameInsertField.get<int>(gkX0, 0);
    const int kY0 = nameInsertField.get<int>(gkY0, 0);
    NameInf_.insertPos = {nameInsertField.get_child(gkCursor).get<int>(gkX0),
                          nameInsertField.get_child(gkCursor).get<int>(gkY0)};
    const boost::property_tree::ptree kFields = nameInsertField.get_child(gkInsertNameFieldFields);
    for( const auto& kField: kFields ) {
      const int kX = kField.second.get<int>(gkX0, 0);
      const int kY = kField.second.get<int>(gkY0, 0);
      const auto name = kField.second.get<std::string>(gkField);
      mvwaddstr(window_, screenSize_.Y0 + kY0 + kY, screenSize_.X0 + kX0 + kX, name.data());
      wrefresh(window_);
    }
    mvwaddstr(window_, screenSize_.Y0 + NameInf_.insertPos.second,
              screenSize_.X0 + NameInf_.insertPos.first, NameInf_.name.data());
    wrefresh(window_);
  }

  void gameMenu::drawScoreTable(boost::property_tree::ptree&& scoreTable,
                                const std::string& scorePath) {
    const std::pair<int, int> kScoreTableCoords = {screenSize_.X0 + scoreTable.get<int>(gkX0),
                                                   screenSize_.Y0 + scoreTable.get<int>(gkY0)};

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


    const auto scoreTableDraw = scoreTable.get<std::string>(gkScoreTable);
    for( int y = 0; y < kScoreBorderHeight; y++ ) {
      mvwaddnstr(window_, kScoreTableCoords.second + y, kScoreTableCoords.first,
                 scoreTableDraw.data() + kScoreBorderWidth * y, kScoreBorderWidth);
      wrefresh(window_);
    }

    const int scoreRowsLimit = scoreTable.get<int>(gkScoreTableRowsLimit);
    int line = 0;
    boost::property_tree::ptree scores;
    boost::property_tree::read_json(gkScorePath, scores);
    for( auto field = scores.get_child(gkScoreTitle).get_child(gkScoreFields).begin();
         field != scores.get_child(gkScoreTitle).get_child(gkScoreFields).end()
         && line < scoreRowsLimit;
         ++field, ++line ) {
      std::string score = std::to_string(field->second.get<int>(gkEnemy1)) + "_"
                        + std::to_string(field->second.get<int>(gkEnemy2)) + "_"
                        + std::to_string(field->second.get<int>(gkEnemy3));
      int precision = kCursorPosScoreMaxLen - std::min<int>(kCursorPosScoreMaxLen, score.size());
      score.insert(0, precision, gkEmptyNameFiller);

      mvwaddnstr(window_, kPlayerNameCoords.second + line, kPlayerNameCoords.first,
                 field->second.get<std::string>(gkPlayerName).data(), kCursorPosNameMaxLen);
      mvwaddnstr(window_, kPlayerScoreCoords.second + line, kPlayerScoreCoords.first, score.data(),
                 kCursorPosScoreMaxLen);
      wrefresh(window_);
    }
    wrefresh(window_);
  }

  void gameMenu::introducePlayerName(const int c) {
    if( c == KEY_ENTER || c == 10 ) {
      NameInf_.name.resize(NameInf_.len);
      ApproveIndex_++;
    }
    else if( (c == KEY_BACKSPACE || c == KEY_DC || c == 127) ) {
      if( NameInf_.len > 0 ) {
        NameInf_.name[--NameInf_.len] = gkEmptyNameFiller;
      }
    }
    else if( NameInf_.len < gkMaxNameLen && c != ERR ) {
      NameInf_.name[NameInf_.len++] = c;
    }
    mvwaddnstr(window_, screenSize_.Y0 + NameInf_.insertPos.second,
               screenSize_.X0 + NameInf_.insertPos.first, NameInf_.name.data(),
               NameInf_.name.size());
    wrefresh(window_);
  }

  std::tuple<std::string, std::vector<int>> gameMenu::getIntroducedInf() {
    return std::make_tuple(NameInf_.name, EnemyInf_.values);
    // todo fix return value;
  }

  void gameMenu::shiftEnemyIntroduceCursor(const int kPrevPos) {
    mvwaddnstr(window_, screenSize_.Y0 + EnemyInf_.cursorPos[kPrevPos].second,
               screenSize_.X0 + EnemyInf_.cursorPos[kPrevPos].first, EnemyInf_.noCursor.data(),
               EnemyInf_.noCursor.size());
    mvwaddnstr(window_, screenSize_.Y0 + EnemyInf_.cursorPos[EnemyInf_.enemyIndex].second,
               screenSize_.X0 + EnemyInf_.cursorPos[EnemyInf_.enemyIndex].first,
               EnemyInf_.cursorAvatar.data(), EnemyInf_.cursorAvatar.size());
  }

  void gameMenu::updateEnemyIntroduceStats() {
    auto inf = std::to_string(EnemyInf_.values[EnemyInf_.enemyIndex]);
    const int width = EnemyInf_.width[EnemyInf_.enemyIndex];
    const int precision = width - std::min<int>(inf.size(), width);
    const int y = EnemyInf_.insertPos[EnemyInf_.enemyIndex].second;
    const int x = EnemyInf_.insertPos[EnemyInf_.enemyIndex].first;
    inf.insert(0, precision, gkEmptyNameFiller);

    mvwaddnstr(window_, screenSize_.Y0 + y, screenSize_.X0 + x, inf.data(), width);
  }

  void gameMenu::introduceEnemy(const int c) {
    if( c == ERR ) {
    }
    else if( c == gkCursorUp ) {
      int x = EnemyInf_.enemyIndex;
      EnemyInf_.enemyIndex =
              (EnemyInf_.enemyIndex > 0) ? --EnemyInf_.enemyIndex : EnemyInf_.enemyIndex;
      shiftEnemyIntroduceCursor(x);
    }
    else if( c == gkCursorDown ) {
      int x = EnemyInf_.enemyIndex;
      EnemyInf_.enemyIndex = (EnemyInf_.enemyIndex < EnemyInf_.cursorRange - 1)
                                   ? ++EnemyInf_.enemyIndex
                                   : EnemyInf_.enemyIndex;
      shiftEnemyIntroduceCursor(x);
    }
    else if( c == gkStatsUp ) {
      EnemyInf_.values[EnemyInf_.enemyIndex] += 1;
      updateEnemyIntroduceStats();
    }
    else if( c == gkStatsDown ) {
      EnemyInf_.values[EnemyInf_.enemyIndex] <= 0 ? 0 : EnemyInf_.values[EnemyInf_.enemyIndex] -= 1;
      updateEnemyIntroduceStats();
    }
    else if( c == KEY_ENTER || c == 10 ) {
      ApproveIndex_++;
    }
  }

  void gameMenu::drawEnemyQuantity(boost::property_tree::ptree&& json) {
    const boost::property_tree::ptree kFields = json.get_child(gkEnemyChoseFields);
    const int kX0 = json.get<int>(gkX0, 0);
    const int kY0 = json.get<int>(gkY0, 0);
    for( const auto& kField: kFields ) {
      const int kX = kField.second.get<int>(gkX0, 0);
      const int kY = kField.second.get<int>(gkY0, 0);
      const auto name = kField.second.get<std::string>(gkField);
      mvwaddstr(window_, screenSize_.Y0 + kY0 + kY, screenSize_.X0 + kX0 + kX, name.data());
      wrefresh(window_);
    }
    getEnemyQuantityChoseInf(kX0, kY0, std::move(json.get_child(gkEnemyCursorSettings)));
  }

  void gameMenu::getEnemyQuantityChoseInf(const int xIndent, const int yIndent,
                                          boost::property_tree::ptree&& json) {
    const int kX0 = json.get<int>(gkX0, 0);
    const int kY0 = json.get<int>(gkY0, 0);
    EnemyInf_.cursorAvatar = json.get<std::string>(gkCursorAvatar);
    EnemyInf_.noCursor = json.get<std::string>(gkEmptyCursorAvatar);
    EnemyInf_.values.resize(json.get<int>(gkCursorQuantity));
    EnemyInf_.cursorRange = json.get<int>(gkCursorQuantity);
    std::fill(EnemyInf_.values.begin(), EnemyInf_.values.end(), gkEnemyQuantityNotInstalled);
    //    std::fill(EnemyInf_.values.begin(), EnemyInf_.values.end(), 0);

    const boost::property_tree::ptree kFields = json.get_child(gkEnemyCursorInf);
    for( const auto& kField: kFields ) {
      int kX = kField.second.get_child(gkCursorAvatarPos).get<int>(gkX0);
      int kY = kField.second.get_child(gkCursorAvatarPos).get<int>(gkY0);
      EnemyInf_.cursorPos.emplace_back(xIndent + kX0 + kX, yIndent + kY0 + kY);
    }

    for( const auto& kField: kFields ) {
      int kX = kField.second.get_child(gkCursorAvatarInsert).get<int>(gkX0);
      int kY = kField.second.get_child(gkCursorAvatarInsert).get<int>(gkY0);
      EnemyInf_.insertPos.emplace_back(xIndent + kX0 + kX, yIndent + kY0 + kY);
      EnemyInf_.width.push_back(kField.second.get_child(gkCursorAvatarInsert).get<int>(gkWidth));
    }
  }

  int gameMenu::process() {
    int c;
    while( (c = input()) != gkExit ) {
      if( ApproveIndex_ == ApproveIndexes::ekName ) {
        introducePlayerName(c);
      }
      else if( ApproveIndex_ == ApproveIndexes::ekEnemies ) {
        introduceEnemy(c);
      }
      else if( ApproveIndex_ == ApproveIndexes::ekFinish ) {
        return ERR;
      }
    }
    return c;
  }
}// namespace gScreen
