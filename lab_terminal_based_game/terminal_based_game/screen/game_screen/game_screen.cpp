#include "game_screen.hpp"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

namespace {
  const char * gkWidth = "width";
  const char * gkHeight = "height";
  const char * gkX0 = "x";
  const char * gkY0 = "y";
  const char * gkMap = "map";
  const char * kEmptySpace = "empty_space";
  const char * kWall = "wall";

  const char * kGameMap = "game_map";

  const char * kGameStats = "game_stats";
  const char * gkFields = "fields";
  const char * kStatsFieldName = "field_name";
  const char * kStatsFieldWidth = "field_width";

  const char gkEmptyNameFiller = '_';
}// namespace

namespace gScreen {
  gameScreen::gameScreen(const basicScreen& kScreen, const std::string& kSettingsPath)
      : basicScreen(kScreen) {
    boost::property_tree::ptree gameScreenConfig;
    boost::property_tree::read_json(kSettingsPath, gameScreenConfig);

    drawGameMap(std::move(gameScreenConfig.get_child(kGameMap)));
    drawGameStats(std::move(gameScreenConfig.get_child(kGameStats)));
  }

  gameScreen::~gameScreen() {
    initBaseScreen();
  }

  void gameScreen::loadGameStats(boost::property_tree::ptree&& kStats) {
    gameStatsSize_ = windowSettings{
            .width = kStats.get<int>(gkWidth),
            .height = kStats.get<int>(gkHeight),
            .startX = kStats.get<int>(gkX0, 0) + screenSize_.startX,
            .startY = kStats.get<int>(gkY0, 0) + screenSize_.startX,
    };

    for( const auto& statsField: kStats.get_child(gkFields) ) {
      const auto kFieldName = statsField.second.get<std::string>(kStatsFieldName);
      std::pair<int, int> coords = {gameStatsSize_.startX + statsField.second.get<int>(gkX0),
                                    gameStatsSize_.startY + statsField.second.get<int>(gkY0)};
      gameStats_[kFieldName].first = std::move(coords);
      gameStats_[kFieldName].second = statsField.second.get<int>(kStatsFieldWidth);
    }
  }

  void gameScreen::drawGameStats(boost::property_tree::ptree&& kStats) {
    loadGameStats(std::move(kStats));

    for( const auto& statsField: gameStats_ ) {
      const char *key = statsField.first.data();
      const int x = statsField.second.first.first;
      const int y = statsField.second.first.second;
      mvwaddstr(window_, y, x, key);
      wrefresh(window_);
    }
  }

  void gameScreen::updateGameStat(const std::string& key, std::string&& value) {
    const int n = gameStats_[key].second;
    if (n > value.size()){
      value.insert(0, n - value.size(), gkEmptyNameFiller);
    }

    const int x = gameStats_[key].first.first + key.size();
    const int y = gameStats_[key].first.second;
    mvwaddnstr(window_, y, x, value.data(), n);
    wrefresh(window_);
  }

  void gameScreen::updateGameStat(const std::string& key, int value) {
    std::string strValue = std::to_string(value);
    updateGameStat(key, std::move(strValue));
  }

  void gameScreen::loadGameMap(boost::property_tree::ptree&& kMapSettings) {
    gameMapSize_ = windowSettings{
            .width = kMapSettings.get<int>(gkWidth),
            .height = kMapSettings.get<int>(gkHeight),
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

  void gameScreen::drawGameMap(boost::property_tree::ptree&& kMapSettings) {
    loadGameMap(std::move(kMapSettings));
    for( int y = 0; y < gameMapSize_.height; ++y ) {
      mvwaddnstr(window_, gameMapSize_.startY + y, gameMapSize_.startX,
                 gameMap_.data() + y * gameMapSize_.width, gameMapSize_.width);
      wrefresh(window_);
    }
  }

  void gameScreen::deleteObj(const std::pair<int, int>& objectCoords) {
    mvwaddch(window_, gameMapSize_.startY + objectCoords.second,
             gameMapSize_.startX + objectCoords.first,
             gameMap_[objectCoords.first + objectCoords.second * gameMapSize_.width]);

    wrefresh(window_);
  }

  bool gameScreen::fixCollision(const std::pair<int, int>& objectCoords,
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
      while( objectCoords.first < -objectShift.first ) {
        ++objectShift.first;
      }

      while( objectCoords.first + objectShift.first >= gameMapSize_.width ) {
        --objectShift.first;
      }

      while( objectCoords.second < -objectShift.second ) {
        ++objectShift.second;
      }

      while( objectCoords.second + objectShift.second >= gameMapSize_.width ) {
        --objectShift.second;
      }

      while( gameMap_[(objectCoords.first + objectShift.first)
                      + (objectCoords.second) * gameMapSize_.width]
             != emptySpace_ ) {
        (objectShift.first > 0) ? --objectShift.first : ++objectShift.first;
      }

      while( gameMap_[(objectCoords.first)
                      + (objectCoords.second + objectShift.second) * gameMapSize_.width]
             != emptySpace_ ) {
        (objectShift.second > 0) ? --objectShift.second : ++objectShift.second;
      }
      return true;
    }
  }

  windowSettings gameScreen::GetMapSize() const noexcept {
    return gameMapSize_;
  }
}// namespace gScreen
