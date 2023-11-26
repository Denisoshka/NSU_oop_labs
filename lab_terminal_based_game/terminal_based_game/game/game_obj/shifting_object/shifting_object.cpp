#include "shifting_object.hpp"

#include <algorithm>

namespace gameObj {
  ShiftingObject::ShiftingObject(ObjDirection kViewDirection,
                                 const std::pair<int, int>& kStartCoords, char kAvatar,
                                 const int kLivesQuantity, const int kBattleDamage,
                                 const ObjectFraction kFraction,
                                 const ObjectProtection kProtection_, const ObjectType kType)
      : Avatar_(std::string{kAvatar})
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

  void ShiftingObject::action(std::vector<std::shared_ptr<gameObj::ShiftingObject>>& objects,
                              std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    for( auto& object: objects ) {
      if( this == &(*object) || !isCollision(object) ) {
        continue;
      }
      interaction(object, trace);
    }
  }

  bool ShiftingObject::isCollision(const std::shared_ptr<gameObj::ShiftingObject>& other) {
    bool flag = NewCoreCoords_ == other->NewCoreCoords_;
    for( auto cords = NewCoords_.begin(); cords != NewCoords_.end() && !flag; ++cords ) {
      flag = std::ranges::any_of(other->NewCoords_.begin(), other->NewCoords_.end(),
                                 [&cords](auto& x) { return x == *cords; });
    }
    return flag;
  }

  const std::string& ShiftingObject::getAvatar() {
    return Avatar_;
  }

  const std::vector<std::pair<int, int>>& ShiftingObject::getCoords() {
    return Coords_;
  }

  const std::vector<std::pair<int, int>>& ShiftingObject::getNewCoords() {
    return NewCoords_;
  }

  const std::pair<int, int>& ShiftingObject::getCoreCords() {
    return CoreCoords_;
  }

  const std::pair<int, int>& ShiftingObject::getNewCoreCords() {
    return NewCoreCoords_;
  }
}// namespace gameObj
