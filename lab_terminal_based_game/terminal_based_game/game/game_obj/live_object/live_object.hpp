#pragma once

#include "shifting_object.hpp"

#include <chrono>

namespace gameObj {
  class LiveObject: public ShiftingObject {
  public:
    LiveObject(ObjDirection kViewDirection, const std::pair<int, int>& kStartCoords, char kAvatar,
               const int kLivesQuantity, const int kBattleDamage, const ObjectFraction kFraction,
               const ObjectProtection kProtection_, const ObjectType kType);
    virtual ~LiveObject() = default;

  protected:
    int UsesForMove_;
    int AttemptsToShift_;
    WeaponConditions WeaponCond_;
    std::chrono::time_point<std::chrono::steady_clock> LastMove_;
    std::chrono::time_point<std::chrono::steady_clock> LastShoot_;
  };
}// namespace gameObj
