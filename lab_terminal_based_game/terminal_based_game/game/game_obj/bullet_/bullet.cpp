#include "bullet.hpp"

namespace {
  const std::pair<int, int> bulletDirectionShift{0, 1};
  const int ekBulletLivesQuantity = 1;
  const int ekBulletDamage  = 1;
}// namespace

namespace gameObj {
  Bullet::Bullet(const ObjDirection viewDirection, const std::pair<int, int>& startCoords)
      : ShiftingObject(viewDirection, startCoords, '*', ekBulletLivesQuantity, ekBulletDamage) {
    if( viewDirection_ == ekObjUp ) {
      directionShift_ =
              std::move(std::pair{bulletDirectionShift.first, -bulletDirectionShift.second});
    }
    else if( viewDirection_ == ekOBJDown ) {
      directionShift_ =
              std::move(std::pair{bulletDirectionShift.first, bulletDirectionShift.second});
    }
  }

  std::pair<int, int> Bullet::desiredShift() const {
    /// todo нужно сделать передвижение через определенное время
    return directionShift_;
  }

}// namespace gameObj