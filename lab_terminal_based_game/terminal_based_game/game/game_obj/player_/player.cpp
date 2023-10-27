#include "player.hpp"
#include "bullet.hpp"

#include <cstdint>
#include <memory>

namespace {
  const char kMoveLeft{'a'};
  const char kMoveRight{'d'};
  const char kReload{'s'};
  const char kShoot{'s'};
  //  std::pair<unsigned, unsigned>
}// namespace

namespace gameObj {
  Player::Player(unsigned int DirectionDegrees, std::pair<unsigned int, unsigned int>&& startCoords)
      : ShiftingObject(0, std::move(startCoords)) {
  }

  std::shared_ptr<basicObj> Player::action(const char action) {
    if( action == kMoveLeft ) {
      DirectionShift_ = std::pair{-1, 0};
    }
    else if( action == kMoveRight ) {
      DirectionShift_ = std::pair{1, 0};
    }
    else if( action == kReload ) {
      //      todo make reload;
    }
    else if( action == kShoot ) {

      std::pair<unsigned, unsigned> bulletCoords{Coords_.first, Coords_.second + Direction_};
      return std::make_unique<Bullet>(Direction_, );
    }
    return nullptr;
  }
}// namespace game_obj