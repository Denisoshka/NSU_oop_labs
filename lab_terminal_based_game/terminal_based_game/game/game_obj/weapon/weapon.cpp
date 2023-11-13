#include "weapon.hpp"

namespace gameObj {
  Weapon::Weapon(ObjDirection kViewDirection, const std::pair<int, int>& kStartCoords, char kAvatar,
                 const int kLivesQuantity, const int kBattleDamage, const ObjectFraction kFraction,
                 const ObjectProtection kProtection, const int kUsesPerFrame)
      : ShiftingObject(kViewDirection, kStartCoords, kAvatar, kLivesQuantity, kBattleDamage,
                       kFraction, kProtection, ObjectType::ekWeaponType)
      , UsesPerFrame_(kUsesPerFrame)
      , LastMoveTime_(std::chrono::steady_clock::now()) {
  }
}// namespace gameObj
