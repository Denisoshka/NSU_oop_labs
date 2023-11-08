#include "game_screen.hpp"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/tokenizer.hpp>

namespace {
  std::string gkWidthName = "width";
  std::string gkHeightName = "height";
  std::string gkX0 = "x";
  std::string gkY0 = "y";
  std::string gkMap = "map";
  std::string kEmptySpace = "empty_space";
  std::string kWall = "wall";

  std::string kGameMap = "game_map";

  std::string kGameStats = "game_stats";
  std::string kStatsFieldName = "field_name";
  std::string kStatsFieldWidth = "field_width";
}// namespace

namespace gScreen {
  gameScreen::gameScreen(const basicScreen& kScreen, const std::string& kSettingsPath)
      : basicScreen(kScreen) {
    boost::property_tree::ptree gameScreenConfig;
    boost::property_tree::read_json(kSettingsPath, gameScreenConfig);

    loadGameMap(std::move(gameScreenConfig.get_child(kGameMap)));
    loadGameStats(std::move(gameScreenConfig.get_child(kGameStats)));
    drawGameMap();
    drawGameStats();
  }

  gameScreen::~gameScreen() {
    initBaseScreen();
  }

  void gameScreen::loadGameStats(boost::property_tree::ptree&& kStats) {
    gameStatsSize_ = subWindowSize{
            .width = kStats.get<int>(gkWidthName),
            .height = kStats.get<int>(gkHeightName),
            .startX = kStats.get<int>(gkX0, 0) + screenSize_.startX,
            .startY = kStats.get<int>(gkY0, 0) + screenSize_.startX,
    };
    const boost::property_tree::ptree& kStatsFields = kStats.get_child(kGameStats);

    for( const auto& statsField: kStatsFields ) {
      const auto kFieldName = statsField.second.get<std::string>(kStatsFieldName);
      std::pair<int, int> coords = {
              gameStatsSize_.startX + statsField.second.get<int>(gkX0),
              gameStatsSize_.startY + statsField.second.get<int>(gkY0)};
      gameStats_[kFieldName] = std::move(coords);
    }
  }

  void gameScreen::drawGameStats() {
    for( const auto& statsField: gameStats_ ) {
      const char *key = statsField.first.data();
      const int x = statsField.second.first;
      const int y = statsField.second.second;
      mvwaddstr(window_, y, x, key);
      wrefresh(window_);
    }
  }

  void gameScreen::updateGameStat(const std::string& key, std::string&& value) {
    const int x = gameStats_[key].first + key.size();
    const int y = gameStats_[key].second;
    mvwaddstr(window_, y, x, value.data());
    wrefresh(window_);
  }

  void gameScreen::loadGameMap( boost::property_tree::ptree&& kMapSettings) {
    gameMapSize_ = subWindowSize{
            .width = kMapSettings.get<int>(gkWidthName),
            .height = kMapSettings.get<int>(gkHeightName),
            .startX = kMapSettings.get<int>(gkX0, 0) + screenSize_.startX,
            .startY = kMapSettings.get<int>(gkY0, 0) + screenSize_.startX,
    };
    gameMap_ = kMapSettings.get<std::string>(gkMap);
    emptySpace_ = kMapSettings.get<char>(kEmptySpace);
  }

  void gameScreen::drawMoveGameObj(const std::pair<int, int>& objectCoords,
                                   const std::pair<int, int>& objectShift, const char avatar) {
    mvwaddch(window_, gameMapSize_.startY + objectCoords.second,
             gameMapSize_.startX + objectCoords.first,
             gameMap_[objectCoords.first + objectCoords.second * gameMapSize_.width]);
    mvwaddch(window_, gameMapSize_.startY + objectCoords.second + objectShift.second,
             gameMapSize_.startX + objectCoords.first + objectShift.first, avatar);

    wrefresh(window_);
  }

  void gameScreen::drawGameMap() {
    for( int i = 0; i < gameMap_.size(); ++i ) {
      mvwaddch(window_, (i / gameMapSize_.width) + gameMapSize_.startY,
               (i % gameMapSize_.width) + gameMapSize_.startX, gameMap_[i]);
      wrefresh(window_);
    }
  }

  void gameScreen::deleteGameObj(const std::pair<int, int>& objectCoords) {
    mvwaddch(window_, gameMapSize_.startY + objectCoords.second,
             gameMapSize_.startX + objectCoords.first,
             gameMap_[objectCoords.first + objectCoords.second * gameMapSize_.width]);

    wrefresh(window_);
  }

  bool gameScreen::fixCoordsToMove(const std::pair<int, int>& objectCoords,
                                   std::pair<int, int>& objectShift) {
    if( 0 <= (objectCoords.first + objectShift.first)
        && (objectCoords.first + objectShift.first) < gameMapSize_.width
        && 0 <= (objectCoords.second + objectShift.second)
        && (objectCoords.second + objectShift.second) < gameMapSize_.height
        && gameMap_[(objectCoords.first + objectShift.first)
                    + (objectCoords.second + objectShift.second) * gameMapSize_.width]
                   == emptySpace_ ) {
      return false;
    }
    else {
      if( objectCoords.first + objectShift.first < 0 ) {
        objectShift.first = -objectCoords.first;
      }
      if( objectCoords.first + objectShift.first >= gameMapSize_.width ) {
        objectShift.first = gameMapSize_.width - 1 - objectCoords.first;
      }


      if( objectCoords.second + objectShift.first < 0 ) {
        objectShift.second = -objectCoords.second;
      }
      if( objectCoords.second + objectShift.second >= gameMapSize_.width ) {
        objectShift.second = gameMapSize_.height - 1 - objectCoords.second;
      }

      if( gameMap_[(objectCoords.first + objectShift.first)
                   + (objectCoords.second + objectShift.second) * gameMapSize_.width]
          != emptySpace_ ) {
        objectShift.first = 0;
      }
      if( gameMap_[(objectCoords.first + objectShift.first)
                   + (objectCoords.second + objectShift.second) * gameMapSize_.width]
          == emptySpace_ ) {
        objectShift.second = 0;
      }

      return true;
    }
  }
}// namespace gScreen
