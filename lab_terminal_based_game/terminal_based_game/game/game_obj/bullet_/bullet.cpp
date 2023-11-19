#include "bullet.hpp"

namespace {
  const std::pair<int, int> bulletDirectionShift{0, 1};

  const int elapsedMSToMove = 100;
  const int gkBulletLivesQuantity = 10;
  const int gkBulletDamage = 30;
  const char gkBulletAvatar = '|';
  const int gkBulletUsesPerMove = 6;
}// namespace

namespace gameObj {
  Bullet::Bullet(const ObjDirection viewDirection, const std::pair<int, int>& startCoords,
                 const ObjectFraction fraction)
      : Weapon(viewDirection, startCoords, gkBulletAvatar, gkBulletLivesQuantity, gkBulletDamage,
               fraction, ObjectProtection::ekBaseProtection, gkBulletUsesPerMove) {
    Shift_ = bulletDirectionShift;
    if( viewDirection == ObjDirection::ekObjUp ) {
      Shift_.second = -Shift_.second;
    }
  }

  void Bullet::updateCondition(std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    CoreCoords_ = NewCoreCoords_;
    Coords_.front() = NewCoords_.front();
    WeaponCond_ = (WeaponCond_ != WeaponConditions::ekNewWeapon) ? WeaponConditions::ekUnUsable
                                                                 : WeaponConditions::ekNewWeapon;

    auto curTime = std::chrono::steady_clock::now();
    if( std::chrono::duration_cast<std::chrono::milliseconds>(curTime - LastMove_).count()
        >= elapsedMSToMove ) {
      RotationEnd_ = false;
      LastMove_ = curTime;
      WeaponCond_ = WeaponConditions::ekNewWeapon;
      UsesForMove_ = gkBulletUsesPerMove;
    }
  }

  bool Bullet::getFight(std::shared_ptr<gameObj::ShiftingObject>& enemy,
                        std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    return !isAlive();
  }

  int Bullet::sayDamage(const ShiftingObject& object) const {
    if( (object.getFraction() == Fraction_
         && object.getFraction() != ObjectFraction::ekNoneFraction)
        || UsesForMove_ <= 0 || WeaponCond_ == WeaponConditions::ekUnUsable ) {
      return 0;
    }

    if( BattleDamage_ < object.getProtection() ) {
      return BattleDamage_ / 2;
    }
    else {
      return BattleDamage_;
    }
  }

  int Bullet::getDamage(const ShiftingObject& object) {
    if( (object.getFraction() == Fraction_
         && object.getFraction() != ObjectFraction::ekNoneFraction)
        || WeaponCond_ == WeaponConditions::ekUnUsable ) {
      return 0;
    }

    if( BattleDamage_ < object.getProtection() ) {
      UsesForMove_ -= LivesQuantity_ / 2 + 1;
      LivesQuantity_ -= LivesQuantity_ / 2 + 1;
      WeaponCond_ = WeaponConditions::ekWasInUse;
      return BattleDamage_ / 2;
    }
    --UsesForMove_;
    --LivesQuantity_;
    WeaponCond_ = WeaponConditions::ekWasInUse;
    return BattleDamage_;
  }

  void Bullet::interaction(std::shared_ptr<gameObj::ShiftingObject>& other,
                           std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    getFight(other, trace);
  }

  bool Bullet::checkRoute(const std::vector<std::pair<bool, bool>>& allowedShift) {
    if( allowedShift.front() != std::pair{true, true} ) {
      NewCoreCoords_ = CoreCoords_;
      NewCoords_.front() = Coords_.front();
      LivesQuantity_ = 0;
    }
    return RotationEnd_ = true;
  }

  const std::vector<std::pair<int, int>>& Bullet::offerNewCoords() {
    CoreCoords_.second += Shift_.second;
    NewCoords_.front().second += Shift_.second;
    return NewCoords_;
  }
}// namespace gameObj
