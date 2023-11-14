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
      , LivesQuantity_(kLivesQuantity)
      , BattleDamage_(kBattleDamage)
      , Shift_({0, 0}) {
  }

  bool ShiftingObject::isAlive() const {
    return LivesQuantity_ > 0;
  }

  ObjectFraction ShiftingObject::getFraction() const {
    return Fraction_;
  }

  ObjectProtection ShiftingObject::getProtection() const {
    return Protection_;
  }

  ObjectType ShiftingObject::getType() const {
    return Type_;
  }

  bool ShiftingObject::rotationEnd() const {
    return RotationEnd_;
  }

  int ShiftingObject::getLivesQuantity() const {
    return LivesQuantity_;
  }

  int ShiftingObject::getDamage() const {
    return BattleDamage_;
  }

  bool ShiftingObject::isCollision(const ShiftingObject& object) {
    return CoreCoords_ == object.CoreCoords_;
  }

  void ShiftingObject::updateCondition(
          std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    RotationEnd_ = false;
    CoreCoords_ = NewCoreCoords_;
    std::copy(Coords_.begin(), Coords_.end(), NewCoords_.begin());
  }


}// namespace gameObj
