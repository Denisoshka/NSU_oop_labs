#include "game_menu.hpp"
#include <fstream>

namespace {
  const int kScoreBorderSize = 17;
  const int kGameNameXIndent = kScoreBorderSize + 7;
  const int kNameInsertTitleYIndent = kGameNameXIndent + 3;
  const int kNameInsertTitleXIndent = kGameNameXIndent;
  const int kNameInsertFieldYIndent = kNameInsertTitleYIndent + 2;
  const int kGameNameYIndent = 3;
  const int kScoreLinesQuantity = 10;
  const int kScoreMaxNameLen = 10;
  const int kScoreMaxTimeLen = 5;
  const int kScoreXTimeStart = 13;
  const int kMaxStatsQuantity = 10;

  const std::string kGameName = "CrashDollar * * * ";
  const std::string kInsertNameTitle = "Insert you name ^_^";
  const std::string kInsertNameField = "-> ";

  const char kScoreTooLongNamePass = '_';
  const char kScoreWBorder = '|';
  const char kScoreHBorder = '-';
  const char kScoreTimeFiller = '_';
  const char scoreSeparator = '|';

}// namespace

namespace screeen {
  gameMenu::gameMenu(const basicScreen& screen, const std::vector<int>& gameMenuSize)
      : basicScreen(screen)
      , gameScoreSize_(screenSize_) {
  }

  void gameMenu::drawGameMenu(const std::vector<std::pair<std::string, int>>& score) {
    drawScoreTable(score);
    drawGameName();
  }

  void gameMenu::drawGameName() {
    mvwaddstr(window_, screenSize_.startY + kGameNameYIndent, screenSize_.startX + kGameNameXIndent,
              kGameName.data());
  }

  void gameMenu::drawNameInsertField() {
    mvwaddstr(window_, screenSize_.startY + kNameInsertTitleYIndent,
              screenSize_.startX + kNameInsertTitleXIndent, kInsertNameTitle.data());
    mvwaddstr(window_, screenSize_.startY + kNameInsertFieldYIndent,
              screenSize_.startX + kNameInsertTitleXIndent, kInsertNameField.data());
  }

  void gameMenu::drawScoreTable(const std::vector<std::pair<std::string, int>>& score) {
    const int kScoreXBorderStart = screenSize_.startY + 1;
    const int kScoreHBorderStart = screenSize_.startX + 1;

    mvwhline(window_, screenSize_.startY, kScoreHBorderStart, kScoreHBorder, kScoreBorderSize);
    mvwhline(window_, kScoreXBorderStart + kScoreLinesQuantity, kScoreHBorderStart, kScoreHBorder,
             kScoreBorderSize);
    mvwvline(window_, kScoreXBorderStart, screenSize_.startX, kScoreWBorder, kScoreLinesQuantity);
    mvwvline(window_, kScoreXBorderStart, kScoreHBorderStart + kScoreBorderSize, kScoreWBorder,
             kScoreLinesQuantity);

    int scoreLineIndex = 0;
    for( const auto& scoreLine: score ) {
      std::string time = std::to_string(scoreLine.second);
      int precision = kScoreMaxTimeLen - std::min<int>(kScoreMaxTimeLen, time.size());
      time.insert(0, precision, kScoreTimeFiller);


      mvwaddnstr(window_, kScoreXBorderStart + scoreLineIndex, kScoreHBorderStart,
                 scoreLine.first.data(), kScoreMaxNameLen);
      mvwaddstr(window_, kScoreXBorderStart + scoreLineIndex, kScoreHBorderStart + kScoreXTimeStart,
                time.data());
      if( scoreLine.first.size() > kScoreMaxNameLen ) {
        mvwaddch(window_, kScoreXBorderStart + scoreLineIndex,
                 kScoreXBorderStart + kScoreMaxNameLen + 1, kScoreTooLongNamePass);
      }
    }
  }

}// namespace screeen