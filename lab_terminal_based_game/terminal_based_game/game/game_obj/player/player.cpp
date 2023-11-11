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
  Player::Player(ObjDirection viewDirection, const std::pair<int, int>& startCoords)
      : ShiftingObject(viewDirection, startCoords, '@', gkPlayerLiverQuantity, gkPlayerDamage) {
  }

  std::shared_ptr<ShiftingObject> Player::action(const int action) {
    DirectionShift_.first = 0;
    if( action == gkMoveLeft ) {
      DirectionShift_.first = -1;
    }
    else if( action == gkMoveRight ) {
      DirectionShift_.first = 1;
    }
    else if( action == gkReload ) {
      AmmoQuantity_ = gkMaxAmmoQuantity;
      //      todo make reload;
    }
    else if( action == gkShoot ) {
      if( AmmoQuantity_ ) {
        std::pair bulletCoords{Coords_.first, Coords_.second + ViewDirection_};
        AmmoQuantity_--;
        return std::make_unique<Bullet>(ViewDirection_, bulletCoords);
      }
      return nullptr;
    }
    return nullptr;
  }

  std::pair<int, int> Player::desiredShift() const {
    return DirectionShift_;
  }

  int Player::getAmmoQuantity() const {
    return AmmoQuantity_;
  }

}// namespace gameObj
