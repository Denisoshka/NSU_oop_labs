#include "bullet.hpp"

namespace {
  const std::pair<int, int> bulletDirectionShift{0, 1};
  const int ekBulletLivesQuantity = 1;
  const int ekBulletDamage = 1;
}// namespace

namespace gameObj {
  Bullet::Bullet(const ObjDirection viewDirection, const std::pair<int, int>& startCoords)
      : ShiftingObject(viewDirection, startCoords, '*', ekBulletLivesQuantity, ekBulletDamage) {
    if( ViewDirection_ == ekObjUp ) {
      DirectionShift_ =
              std::move(std::pair{bulletDirectionShift.first, -bulletDirectionShift.second});
    }
    else if( ViewDirection_ == ekOBJDown ) {
      DirectionShift_ =
              std::move(std::pair{bulletDirectionShift.first, bulletDirectionShift.second});
    }
  }

  std::pair<int, int> Bullet::desiredShift() const {
    /// todo нужно сделать передвижение через определенное время
    return DirectionShift_;
  }

}// namespace gameObj