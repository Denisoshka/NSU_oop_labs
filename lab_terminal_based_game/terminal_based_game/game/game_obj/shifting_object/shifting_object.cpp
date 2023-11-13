#include "shifting_object.hpp"

namespace gameObj {
  ShiftingObject::ShiftingObject(ObjDirection kViewDirection,
                                 const std::pair<int, int>& kStartCoords, char kAvatar,
                                 const int kLivesQuantity, const int kBattleDamage,
                                 const ObjectFraction kFraction,
                                 const ObjectProtection kProtection_, const ObjectType kType)
      : BasicObj(kStartCoords, kAvatar)
      , Fraction_(kFraction)
      , Protection_(kProtection_)
      , Type_(kType)
      , ViewDirection_(kViewDirection)
      , DirectionShift_({0, 0})
      , LivesQuantity_(kLivesQuantity)
      , BattleDamage_(kBattleDamage) {
  }

  void ShiftingObject::makeShift(std::pair<int, int>& allowedShift) {
    Coords_.first += allowedShift.first;
    Coords_.second += allowedShift.second;
  }

  bool ShiftingObject::isAlive() const {
    return LivesQuantity_ > 0;
  }


  ObjectProtection ShiftingObject::getProtection() const {
    return Protection_;
  }

  int ShiftingObject::getLivesQuantity() const {
    return LivesQuantity_;
  }

  ObjectFraction ShiftingObject::getFraction() const {
    return Fraction_;
  }

  std::pair<int, int> ShiftingObject::desiredShift() const {
    return DirectionShift_;
  }

  ObjectType ShiftingObject::getType() const {
    return Type_;
  }

  int ShiftingObject::getDamage() const {
    return BattleDamage_;
  }

  /*  bool ShiftingObject::fight(ShiftingObject& object,
                             std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    LivesQuantity_ -= object.BattleDamage_;
    return !isAlive();
  }*/

  /* void ShiftingObject::updateCondition() {
  }
*/
  /*
  std::shared_ptr<ShiftingObject> ShiftingObject::action(
          const int action, std::vector<std::shared_ptr<gameObj::ShiftingObject>> & objects) {
    for (auto){}
    return nullptr;
  }
*/

}// namespace gameObj
