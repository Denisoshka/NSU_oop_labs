#include "player.hpp"
#include "bullet.hpp"

#include <memory>

namespace {
  const int gkMoveLeft{'a'};
  const int gkMoveRight{'d'};
  const int gkReload{'s'};
  const int gkShoot{'w'};
  const int gkMaxAmmoQuantity = 10;
  const int gkPlayerLiverQuantity = 200;
  const int gkPlayerDamage = 50;
  const int gkReloadMilisecondsTimeout = 250;

  const float gkBasicLowerCoef = 0.7;
  //  std::pair<unsigned, unsigned>
}// namespace

namespace gameObj {
  Player::Player(ObjDirection viewDirection, const std::pair<int, int>& startCoords)
      : ShiftingObject(viewDirection, startCoords, '@', gkPlayerLiverQuantity, gkPlayerDamage,
                       ObjectFraction::ekPlayerFraction, ObjectProtection::ekNoneProtection,
                       ObjectType::ekLiveObjectType) {
  }

  void Player::updateCondition(const int action,
                               std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    CoreCoords_ = NewCoreCoords_;
    Coords_.front() = NewCoords_.front();
    auto curTime = std::chrono::steady_clock::now();
    if( std::chrono::duration_cast<std::chrono::milliseconds>(curTime - LastWeaponReload_).count()
        >= gkReloadMilisecondsTimeout ) {
      WeaponCond_ = WeaponConditions::ekUnUsable;
    }

    Shift_.first = 0;
    if( action == gkMoveLeft ) {
      Shift_.first = -1;
    }
    else if( action == gkMoveRight ) {
      Shift_.first = 1;
    }
    else if( action == gkReload ) {
      AmmoQuantity_ = gkMaxAmmoQuantity;
      //      todo make reload;
    }
    else if( action == gkShoot ) {
      if( AmmoQuantity_ ) {
        std::pair bulletCoords{CoreCoords_.first, CoreCoords_.second + ViewDirection_};
        AmmoQuantity_--;
        trace.push_back(std::make_shared<Bullet>(ViewDirection_, bulletCoords, Fraction_));
      }
    }

    RotationEnd_ = !Shift_.first;
  }

  int Player::getAmmoQuantity() const {
    return AmmoQuantity_;
  }

  void Player::interaction(std::shared_ptr<gameObj::ShiftingObject>& other,
                           std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    getFight(other, trace);
  }

  bool Player::getFight(std::shared_ptr<gameObj::ShiftingObject>& enemy,
                        std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {

    LivesQuantity_ -= enemy->getDamage(*this);
    return isAlive();
  }

  void Player::updateCondition(std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
  }

  bool Player::checkRoute(const std::vector<std::pair<bool, bool>>& allowedShift) {
    if( allowedShift.front() != std::pair{true, true} ) {
      NewCoreCoords_ = CoreCoords_;
      NewCoords_.front() = Coords_.front();
    }

    return RotationEnd_ = true;
  }

  const std::vector<std::pair<int, int>>& Player::offerNewCoords() {
    NewCoreCoords_.first += Shift_.first;
    NewCoords_.front().first += Shift_.first;
    return NewCoords_;
  }

  int Player::sayDamage(const ShiftingObject& object) const {
    if( object.getFraction() == Fraction_ || object.getFraction() != ObjectFraction::ekNoneFraction
        || WeaponCond_ == WeaponConditions::ekUnUsable ) {
      return 0;
    }
    if( Protection_ < object.sayDamage(*this) ) {
      return BattleDamage_ * gkBasicLowerCoef;
    }
    return BattleDamage_;
  }

  int Player::getDamage(const ShiftingObject& object) {
    if( object.getFraction() == Fraction_ || object.getFraction() != ObjectFraction::ekNoneFraction
        || WeaponCond_ == WeaponConditions::ekUnUsable ) {
      return 0;
    }

    if( Protection_ < object.sayDamage(*this) ) {
      WeaponCond_ = WeaponConditions::ekWasInUse;
      return BattleDamage_ * gkBasicLowerCoef;
    }

    WeaponCond_ = WeaponConditions::ekWasInUse;
    return BattleDamage_;
  }
}// namespace gameObj
