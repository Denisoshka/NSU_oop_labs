#include "bullet_destroyer.hpp"

#include <algorithm>
#include <random>

void updateCondition(std::vector<gameObj::ShiftingObject>& trace);

namespace {
  const char gkAvatar = '~';
  const int gkLivesQuantity = 400;
  const int kBattleDamage = 50;

  const int gkStandardMoveTimeout = 750;
  const int gkDamageTimeout = 500;
  const float gkChangeDirChance = 0.4;
  const float gkBasicLowerCoef = 0.4;

  const std::pair<int, int> gkStandardShift = {2, 0};
}// namespace

static bool getRandomBoolean(double probability) {
  srandom(time(nullptr));
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> dis(0.0, 1.0);

  double randomValue = dis(gen);

  return (randomValue < probability);
}

namespace gameObj {
  gameObj::BulletDestroyer::BulletDestroyer(const gameObj::ObjDirection kViewDirection,
                                            const std::pair<int, int>& kStartCoords,
                                            const gameObj::ObjectFraction kFraction)
      : LiveObject(kViewDirection, kStartCoords, gkAvatar, gkLivesQuantity, kBattleDamage,
                   kFraction, ObjectProtection::ekShieldProtection, ObjectType::ekLiveObjectType) {
    Coords_ = {
            {kStartCoords.first - 1, kStartCoords.second},
            {kStartCoords.first,     kStartCoords.second},
            {kStartCoords.first + 1, kStartCoords.second}
    };
    NewCoords_ = Coords_;
    Avatar_ = "~~~";
  }

  void BulletDestroyer::updateCondition(
          std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    CoreCoords_ = NewCoreCoords_;
    Coords_ = NewCoords_;

    const auto kCurTime = std::chrono::steady_clock::now();
    if( std::chrono::duration_cast<std::chrono::milliseconds>(kCurTime - LastShoot_).count()
        >= gkDamageTimeout ) {
      WeaponCond_ = WeaponConditions::ekNewWeapon;
      LastShoot_ = kCurTime;
    }

    manageNextRotations(trace, kCurTime);
  }

  void BulletDestroyer::manageNextRotations(
          std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace,
          const std::chrono::time_point<std::chrono::steady_clock> kCurTime) {
    if( std::chrono::duration_cast<std::chrono::milliseconds>(kCurTime - LastMove_).count()
        >= gkStandardMoveTimeout ) {
      RotationEnd_ = false;
      LastMove_ = kCurTime;
    }

    if( !Shift_.first ) {
      Shift_.first = getRandomBoolean(0.5) ? -gkStandardShift.first : gkStandardShift.first;
    }
    else {
      Shift_.first = (getRandomBoolean(gkChangeDirChance) ? (-Shift_.first) : Shift_.first / 1)
                   * gkStandardShift.first / std::abs(Shift_.first);
    }
  }

  void BulletDestroyer::interaction(std::shared_ptr<gameObj::ShiftingObject>& other,
                                    std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    getFight(other, trace);
  }

  bool BulletDestroyer::getFight(std::shared_ptr<gameObj::ShiftingObject>& enemy,
                                 std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    int damage = enemy->getDamage(*this);
    LivesQuantity_ -= damage;
    return isAlive();
  }

  int BulletDestroyer::sayDamage(const ShiftingObject& object) const {
    if( (object.getFraction() == Fraction_
         && object.getFraction() != ObjectFraction::ekNoneFraction)
        || WeaponCond_ == WeaponConditions::ekUnUsable ) {
      return 0;
    }

    if( BattleDamage_ < object.getProtection())  {
      return BattleDamage_ * gkBasicLowerCoef;
    }
    return BattleDamage_;
  }

  int BulletDestroyer::getDamage(const ShiftingObject& object) {
    if( object.getFraction() == Fraction_ || object.getFraction() != ObjectFraction::ekNoneFraction
        || WeaponCond_ == WeaponConditions::ekUnUsable ) {
      return 0;
    }

    WeaponCond_ = WeaponConditions::ekWasInUse;
    if( BattleDamage_ < object.getProtection() ) {
      return BattleDamage_ * gkBasicLowerCoef;
    }

    return BattleDamage_;
  }

  const std::vector<std::pair<int, int>>& BulletDestroyer::offerNewCoords() {
    NewCoreCoords_.first = CoreCoords_.first + Shift_.first;
    for( int i = 0; i < NewCoords_.size(); ++i ) {
      NewCoords_[i].first = (Coords_[i].first + Shift_.first);
    }
    return NewCoords_;
  }

  bool BulletDestroyer::checkRoute(const std::vector<std::pair<bool, bool>>& allowedShift) {
    if( std::ranges::any_of(allowedShift.begin(), allowedShift.end(), [](auto& x) {
          return x != std::pair{true, true};
        }) ) {
      Shift_.first -= (Shift_.first / std::abs(Shift_.first));
      return RotationEnd_ = !Shift_.first;
    }
    return RotationEnd_ = true;
  }
}// namespace gameObj