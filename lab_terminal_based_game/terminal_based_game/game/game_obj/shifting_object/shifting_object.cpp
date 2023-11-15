#include "shifting_object.hpp"
#include <algorithm>

namespace gameObj {
  ShiftingObject::ShiftingObject(ObjDirection kViewDirection,
                                 const std::pair<int, int>& kStartCoords, char kAvatar,
                                 const int kLivesQuantity, const int kBattleDamage,
                                 const ObjectFraction kFraction,
                                 const ObjectProtection kProtection_, const ObjectType kType)
      : Avatar_(std::vector{kAvatar})
      , CoreCoords_(kStartCoords)
      , Coords_(std::vector{kStartCoords})
      , Fraction_(kFraction)
      , Protection_(kProtection_)
      , Type_(kType)
      , ViewDirection_(kViewDirection)
      , LivesQuantity_(kLivesQuantity)
      , BattleDamage_(kBattleDamage)
      , Shift_({0, 0})
      , NewCoreCoords_(kStartCoords)
      , NewCoords_({kStartCoords}) {
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

  bool ShiftingObject::isCollision(const ShiftingObject& other) {
    bool flag = NewCoreCoords_ == other.NewCoreCoords_;
    for( auto cords = NewCoords_.begin(); cords != NewCoords_.end() && !flag; ++cords ) {
      flag = std::ranges::any_of(other.NewCoords_.begin(), other.NewCoords_.end(),
                                 [&cords](auto& x) { return x == *cords; });
    }
    return flag;
  }

  const std::vector<char>& ShiftingObject::getAvatar() {
    return Avatar_;
  }

  const std::vector<std::pair<int, int>>& ShiftingObject::getCoords() {
    return Coords_;
  }
}// namespace gameObj
