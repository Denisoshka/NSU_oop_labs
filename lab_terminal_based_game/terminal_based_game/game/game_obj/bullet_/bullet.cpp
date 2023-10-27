#include "bullet.hpp"

namespace {
  const unsigned int kDegreesPerHalf_ = 180;
  const std::pair<unsigned, unsigned> BulletDirectionShift{1, 0};
  const int kUp = -1;
  const int kDown = 1;
}// namespace

namespace gameObj {
  Bullet::Bullet(int DirectionDegrees, std::pair<unsigned, unsigned>&& startCoords)
      : ShiftingObject(0, std::move(startCoords)) {
    if( DirectionDegrees % kDegreesPerHalf_ / kDegreesPerHalf_ ) {
      Direction_ = ekObjUp;
      DirectionShift_ = std::move(std::pair{0, BulletDirectionShift.second});
    }
    else {
      Direction_ = ekOBJDown;
      DirectionShift_ = std::move(std::pair{0, -BulletDirectionShift.second});
    }
  }

  std::shared_ptr<basicObj> Bullet::action(const char action) {
  }

  std::pair<unsigned, unsigned> Bullet::desiredShift() const {
    /// todo нужно сделать передвижение через определенное время
    bool statement = true;
    return (statement) ? DirectionShift_ : std::pair{0U, 0U};
  }

  void Bullet::makeShift(std::pair<unsigned, unsigned>& allowedShift) {
    Coords_.first += allowedShift.first;
    Coords_.second += allowedShift.second;
  }
}// namespace gameObj