#include "player.hpp"
#include "bullet.hpp"

#include <cstdint>
#include <memory>

namespace {
  const int gkMoveLeft{'a'};
  const int gkMoveRight{'d'};
  const int gkReload{'s'};
  const int gkShoot{'w'};
  const int gkMaxAmmoQuantity = 10;
  const int gkPlayerLiverQuantity = 5;
  const int gkPlayerDamage = 1;
  //  std::pair<unsigned, unsigned>
}// namespace

namespace gameObj {
  Player::Player( ObjDirection viewDirection,const std::pair< int,  int>& startCoords)
      : ShiftingObject(viewDirection, startCoords, '@', gkPlayerLiverQuantity, gkPlayerDamage) {
  }

  std::shared_ptr<ShiftingObject> Player::action(const int action) {
    directionShift_.first = 0;
    if( action == gkMoveLeft ) {
      directionShift_.first = -1;
    }
    else if( action == gkMoveRight ) {
      directionShift_.first = 1;
    }
    else if( action == gkReload ) {
      ammoQuantity_ = gkMaxAmmoQuantity;
      //      todo make reload;
    }
    else if( action == gkShoot ) {
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
