#pragma once

#include "shifting_object.hpp"

#include <chrono>

namespace gameObj {


  class Weapon: public ShiftingObject {
  public:
    Weapon(ObjDirection kViewDirection, const std::pair<int, int>& kStartCoords, char kAvatar,
           const int kLivesQuantity, const int kBattleDamage, const ObjectFraction kFraction,
           const ObjectProtection kProtection, const int kUsesPerFrame);
    virtual ~Weapon() = default;

  protected:
    WeaponConditions WeaponCond_;
    int UsesForMove_;
    std::chrono::time_point<std::chrono::steady_clock> LastMove_;
  };
}// namespace gameObj
