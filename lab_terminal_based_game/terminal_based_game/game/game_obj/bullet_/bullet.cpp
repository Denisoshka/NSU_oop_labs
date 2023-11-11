#include "bullet.hpp"

namespace {
   const std::pair<int, int> bulletDirectionShift{0, 1};
 }// namespace

namespace gameObj {
  Bullet::Bullet(const ObjDirection viewDirection, const std::pair<int, int>& startCoords)
      : ShiftingObject(ekObjNone, startCoords, '*') {
    viewDirection_ = viewDirection;
    if( viewDirection == ekObjUp) {
      directionShift_ = std::move(std::pair{bulletDirectionShift.first, -bulletDirectionShift.second});
    }
    else if (viewDirection == ekOBJDown){
      directionShift_ = std::move(std::pair{bulletDirectionShift.first, bulletDirectionShift.second});
    }
  }

  /*std::shared_ptr<ShiftingObject> Bullet::action(const int action) {
    return nullptr;
  }*/

  std::pair<int, int> Bullet::desiredShift() const {
    /// todo нужно сделать передвижение через определенное время
    return directionShift_;
  }
/*
  void Bullet::makeShift(std::pair<unsigned, unsigned>& allowedShift) {
    Coords_.first += allowedShift.first;
    Coords_.second += allowedShift.second;
  }*/
}// namespace gameObj