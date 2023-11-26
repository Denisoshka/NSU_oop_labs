#include "weapon.hpp"

namespace {
  const int gkStandartUsesForMove = 1;
}


namespace gameObj {
  Weapon::Weapon(ObjDirection kViewDirection, const std::pair<int, int>& kStartCoords, char kAvatar,
                 const int kLivesQuantity, const int kBattleDamage, const ObjectFraction kFraction,
                 const ObjectProtection kProtection, const int kUsesPerFrame)
      : ShiftingObject(kViewDirection, kStartCoords, kAvatar, kLivesQuantity, kBattleDamage,
                       kFraction, kProtection, ObjectType::ekWeaponType)
      , WeaponCond_(WeaponConditions::ekNewWeapon)
      , UsesForMove_(gkStandartUsesForMove)
      , LastMove_(std::chrono::steady_clock::now()) {
  }
}// namespace gameObj
