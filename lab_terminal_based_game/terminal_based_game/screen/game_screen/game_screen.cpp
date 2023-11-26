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

  const char gkEmptyNameFiller = '_';
}// namespace

namespace gScreen {
  GameScreen::GameScreen(const BasicScreen& kScreen, const std::string& kSettingsPath,
                         const std::string& kGameMap, const windowSettings& kGameMapSize)
      : BasicScreen(kScreen)
      , gameMap_(kGameMap)
      , gameMapSettings_(kGameMapSize) {
    boost::property_tree::ptree gameScreenConfig;
    boost::property_tree::read_json(kSettingsPath, gameScreenConfig);

    centerScreen(gameScreenConfig);

    loadMapScreenSettings(std::move(gameScreenConfig.get_child(kGameMap)));

    loadStatsScreenSettings(std::move(gameScreenConfig.get_child(kGameStats)));
    drawGameStats();
  }

  GameScreen::~GameScreen() {
    initBaseScreen();
  }

  void GameScreen::loadStatsScreenSettings(boost::property_tree::ptree&& settings) {
    gameStatsScreenSettings_ = windowSettings{
            .w = settings.get<int>(gkWidth),
            .h = settings.get<int>(gkHeight),
            .X0 = settings.get<int>(gkX0, 0) + screenSize_.X0,
            .Y0 = settings.get<int>(gkY0, 0) + screenSize_.Y0,
    };

    for( const auto& statsField: settings.get_child(gkFields) ) {
      const auto kFieldName = statsField.second.get<std::string>(kStatsFieldName);
      std::pair<int, int> coords = {gameStatsScreenSettings_.X0 + statsField.second.get<int>(gkX0),
                                    gameStatsScreenSettings_.Y0 + statsField.second.get<int>(gkY0)};
      gameStats_[kFieldName].first = std::move(coords);
      gameStats_[kFieldName].second = statsField.second.get<int>(kStatsFieldWidth);
    }
  }

  void GameScreen::updateMapSettings() {
    gameMapIndentSettings_.X0 =
            gameMapScreenSettings_.X0 + (gameMapScreenSettings_.w - gameMapSettings_.w) / 2;
    gameMapIndentSettings_.Y0 =
            gameMapScreenSettings_.Y0 + (gameMapScreenSettings_.h - gameMapSettings_.h) / 2;
  }

  void GameScreen::drawGameMap() {
    for( int y = 0; y < gameMapSettings_.h; ++y ) {
      mvwaddnstr(window_, gameMapIndentSettings_.Y0 + y, gameMapIndentSettings_.X0,
                 gameMap_.data() + y * gameMapSettings_.w, gameMapSettings_.w);
    }
  }

  void GameScreen::drawGameStats() {
    for( const auto& statsField: gameStats_ ) {
      const char *key = statsField.first.data();
      const int x = statsField.second.first.first;
      const int y = statsField.second.first.second;
      mvwaddstr(window_, y, x, key);
      wrefresh(window_);
    }
  }

  void GameScreen::updateGameStat(const std::string& key, std::string&& value) {
    const int n = gameStats_[key].second;
    if( n > value.size() ) {
      value.insert(0, n - value.size(), gkEmptyNameFiller);
    }

    const int x = gameStats_[key].first.first + key.size();
    const int y = gameStats_[key].first.second;
    mvwaddnstr(window_, y, x, value.data(), n);
    wrefresh(window_);
  }

  void GameScreen::updateGameStat(const std::string& key, int value) {
    std::string strValue = std::to_string(value);
    updateGameStat(key, std::move(strValue));
  }

  void GameScreen::loadMapScreenSettings(boost::property_tree::ptree&& settings) {
    gameMapScreenSettings_ = windowSettings{
            .w = settings.get<int>(gkWidth),
            .h = settings.get<int>(gkHeight),
            .X0 = settings.get<int>(gkX0, 0) + screenSize_.X0,
            .Y0 = settings.get<int>(gkY0, 0) + screenSize_.Y0,
    };
  }

  void GameScreen::drawObj(const std::vector<std::pair<int, int>>& objectCoords,
                           const std::string avatar) {
    for( int i = 0; i < objectCoords.size(); ++i ) {
      mvwaddch(window_, gameMapIndentSettings_.Y0 + objectCoords[i].second,
               gameMapIndentSettings_.X0 + objectCoords[i].first, avatar[i]);
    }
    wrefresh(window_);
  }

  void GameScreen::deleteObj(const std::vector<std::pair<int, int>>& objectCoords) {
    for( auto& coords: objectCoords ) {
      mvwaddch(window_, gameMapIndentSettings_.Y0 + coords.second, gameMapIndentSettings_.X0 + coords.first,
               gameMap_[coords.first + coords.second * gameMapSettings_.w]);
    }
    wrefresh(window_);
  }

  windowSettings GameScreen::getMapSize() const noexcept {
    return gameMapSettings_;
  }
}// namespace gScreen
