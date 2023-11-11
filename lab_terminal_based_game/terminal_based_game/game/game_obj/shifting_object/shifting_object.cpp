#include "shifting_object.hpp"

namespace gameObj {
  ShiftingObject::ShiftingObject(ObjDirection viewDirection, const std::pair<int, int>& startCoords,
                                 char avatar, const int livesQuantity, const int battleDamage_)
      : BasicObj(startCoords, avatar)
      , ViewDirection_(viewDirection)
      , DirectionShift_({0, 0})
      , LivesQuantity_(livesQuantity)
      , BattleDamage_(battleDamage_) {
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
    return LivesQuantity_ > 0;
  }

  bool ShiftingObject::getBattleDamage(gameObj::ShiftingObject& other) noexcept {
    LivesQuantity_ -= other.giveBattleDamage();
    return !this->isAlive();
  }

  int ShiftingObject::giveBattleDamage() {
    return BattleDamage_;
  }

  int ShiftingObject::getLivesQuantity() {
    return LivesQuantity_;
  }

}// namespace gameObj
