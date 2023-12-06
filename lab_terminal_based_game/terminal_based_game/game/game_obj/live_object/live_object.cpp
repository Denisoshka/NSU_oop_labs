#include "live_object.hpp"

gameObj::LiveObject::LiveObject(gameObj::ObjDirection kViewDirection,
                                const std::pair<int, int>& kStartCoords, char kAvatar,
                                const int kLivesQuantity, const int kBattleDamage,
                                const gameObj::ObjectFraction kFraction,
                                const gameObj::ObjectProtection kProtection_,
                                const gameObj::ObjectType kType)
    : ShiftingObject(kViewDirection, kStartCoords, kAvatar, kLivesQuantity, kBattleDamage,
                     kFraction, kProtection_, kType)
    , LastMove_(std::chrono::steady_clock::now())
    , LastShoot_(std::chrono::steady_clock::now())
    , WeaponCond_(WeaponConditions::ekNewWeapon) {
}
