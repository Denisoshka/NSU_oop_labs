#include "player.hpp"
#include "bullet.hpp"

#include <cstdint>
#include <memory>

namespace {
  const char kMoveLeft{'a'};
  const char kMoveRight{'d'};
  const char kReload{'s'};
  const char kShoot{'w'};
  const int maxAmmoQuantity = 10;
  //  std::pair<unsigned, unsigned>
}// namespace

namespace gameObj {
  Player::Player( int DirectionDegrees, std::pair< int,  int>&& startCoords)
      : ShiftingObject(DirectionDegrees, std::move(startCoords), '@') {
  }

  std::shared_ptr<ShiftingObject> Player::action(const int action) {
    DirectionShift_.first = 0;
    if( action == kMoveLeft ) {
      DirectionShift_.first = -1;
    }
    else if( action == kMoveRight ) {
      DirectionShift_.first = 1;
    }
    else if( action == kReload ) {
      ammoQuantity_ = maxAmmoQuantity;
      //      todo make reload;
    }
    else if( action == kShoot ) {
      if (ammoQuantity_){
        std::pair bulletCoords{Coords_.first, Coords_.second + Direction_};
        return std::make_unique<Bullet>(Direction_, std::move(bulletCoords));
      }
      return nullptr;
    }
    return nullptr;
  }

  std::pair<int, int> Player::desiredShift() const {
    return DirectionShift_;
  }
}// namespace gameObj
