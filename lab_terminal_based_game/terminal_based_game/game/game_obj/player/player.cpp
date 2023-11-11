#include "player.hpp"
#include "bullet.hpp"

#include <cstdint>
#include <memory>

namespace {
  const int kMoveLeft{'a'};
  const int kMoveRight{'d'};
  const int kReload{'s'};
  const int kShoot{'w'};
  const int maxAmmoQuantity = 10;
  //  std::pair<unsigned, unsigned>
}// namespace

namespace gameObj {
  Player::Player( ObjDirection viewDirection,const std::pair< int,  int>& startCoords)
      : ShiftingObject(viewDirection, startCoords, '@') {
  }

  std::shared_ptr<ShiftingObject> Player::action(const int action) {
    directionShift_.first = 0;
    if( action == kMoveLeft ) {
      directionShift_.first = -1;
    }
    else if( action == kMoveRight ) {
      directionShift_.first = 1;
    }
    else if( action == kReload ) {
      ammoQuantity_ = maxAmmoQuantity;
      //      todo make reload;
    }
    else if( action == kShoot ) {
      if (ammoQuantity_){
        std::pair bulletCoords{Coords_.first, Coords_.second + viewDirection_};
        ammoQuantity_--;
        return std::make_unique<Bullet>(viewDirection_, bulletCoords);
      }
      return nullptr;
    }
    return nullptr;
  }

  std::pair<int, int> Player::desiredShift() const {
    return directionShift_;
  }

  int Player::getAmmoQuantity() const {
    return ammoQuantity_;
  }
}// namespace gameObj
