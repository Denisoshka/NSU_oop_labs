#pragma once

#include <chrono>

#include "shifting_object.hpp"

namespace gameObj {
  class Weapon: public ShiftingObject {
  public:
    Weapon(ObjDirection kViewDirection, const std::pair<int, int>& kStartCoords, char kAvatar,
           const int kLivesQuantity, const int kBattleDamage, const ObjectFraction kFraction,
           const ObjectProtection kProtection, const int kUsesPerFrame);
  protected:
    int UsesPerMove_;
    std::chrono::time_point<std::chrono::steady_clock> LastMoveTime_;
  };
}// namespace gameObj
