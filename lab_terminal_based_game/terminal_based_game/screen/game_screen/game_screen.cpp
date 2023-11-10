#include "game_screen.hpp"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

namespace {
  const std::string gkWidth = "width";
  const std::string gkHeight = "height";
  const std::string gkX0 = "x";
  const std::string gkY0 = "y";
  const std::string gkMap = "map";
  const std::string kEmptySpace = "empty_space";
  const std::string kWall = "wall";

  const std::string kGameMap = "game_map";

  const std::string kGameStats = "game_stats";
  const std::string gkFields = "fields";
  const std::string kStatsFieldName = "field_name";
  const std::string kStatsFieldWidth = "field_width";
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
      gameStats_[kFieldName] = std::move(coords);
    }
  }

  void gameScreen::drawGameStats(boost::property_tree::ptree&& kStats) {
    loadGameStats(std::move(kStats));

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

  windowSettings gameScreen::GetMapSize() const noexcept {
    return gameMapSize_;
  }
}// namespace gScreen
