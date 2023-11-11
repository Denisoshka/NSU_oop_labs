#include "shifting_object.hpp"

namespace gameObj {
  ShiftingObject::ShiftingObject(ObjDirection viewDirection, const std::pair<int, int>& startCoords,
                                 char avatar, const int livesQuantity, const int battleDamage_)
      : basicObj(startCoords, avatar)
      , viewDirection_(viewDirection)
      , directionShift_({0, 0})
      , livesQuantity_(livesQuantity)
      , battleDamage_(battleDamage_) {
  }

  void ShiftingObject::makeShift(std::pair<int, int>& allowedShift) {
    Coords_.first += allowedShift.first;
    Coords_.second += allowedShift.second;
  }

  std::shared_ptr<ShiftingObject> ShiftingObject::action(const int action) {
    return nullptr;
  }

  bool ShiftingObject::battle(gameObj::ShiftingObject& other) {
    getBattleDamage(other);
    other.getBattleDamage(*this);
    return !this->isAlive();
  }

  bool ShiftingObject::isAlive() {
    return livesQuantity_ > 0;
  }

  bool ShiftingObject::getBattleDamage(gameObj::ShiftingObject& other) noexcept {
    livesQuantity_ -= other.giveBattleDamage();
    return !this->isAlive();
  }

  int ShiftingObject::giveBattleDamage() {
    return battleDamage_;
  }

  int ShiftingObject::getLivesQuantity() {
    return livesQuantity_;
  }

}// namespace gameObj
