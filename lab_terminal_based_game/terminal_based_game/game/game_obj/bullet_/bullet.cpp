#include "bullet.hpp"

namespace {
  const  int kDegreesPerHalf_ = 180;
  const std::pair<int, int> BulletDirectionShift{0, 1};
  const int kUp = -1;
  const int kDown = 1;
}// namespace

namespace gameObj {
  Bullet::Bullet(int DirectionDegrees, std::pair<int, int>&& startCoords)
      : ShiftingObject(0, std::move(startCoords), 0) {
    if( DirectionDegrees % kDegreesPerHalf_ / kDegreesPerHalf_ ) {
      Direction_ = ekObjUp;
      DirectionShift_ = std::move(std::pair{0, BulletDirectionShift.second});
    }
    else {
      Direction_ = ekOBJDown;
      DirectionShift_ = std::move(std::pair{0, -BulletDirectionShift.second});
    }
  }

  /*std::shared_ptr<ShiftingObject> Bullet::action(const int action) {
    return nullptr;
  }*/

  std::pair<int, int> Bullet::desiredShift() const {
    /// todo нужно сделать передвижение через определенное время
    return DirectionShift_;
  }
/*
  void Bullet::makeShift(std::pair<unsigned, unsigned>& allowedShift) {
    Coords_.first += allowedShift.first;
    Coords_.second += allowedShift.second;
  }*/
}// namespace gameObj