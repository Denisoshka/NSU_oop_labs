#include "tracking_enemy.hpp"
#include "bullet.hpp"

#include <algorithm>
#include <random>

namespace {
  const int gkTrackingTimeout = 5000;
  const int gkLivesQuantity = 500;
  const int gkBattleDamage = 20;
  const int gkMaxTrackingCordsDiff = 10;
  const int gkStandardMoveTimeout = 450;
  const int gkTrackingMoveTimeout = 250;
  const int gkDronesPushTimeout = 3000;
  const int gkDronesQuantity = 5;
  const int gkReloadTimeout = 600;
  const int gkBulletsQuantity = 2;

  const float gkChangeDirChance = 0.4;
  const float gkBasicLowerCoef = 0.6;

  const std::pair gkStandardShift = {3, 0};
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
  TrackingEnemy::TrackingEnemy(gameObj::ObjDirection kViewDirection,
                               const std::pair<int, int>& kStartCoords)
      : LiveObject(kViewDirection, kStartCoords, ' ', gkLivesQuantity, gkBattleDamage,
                   ObjectFraction::ekEnemyFraction, ObjectProtection::ekNoneProtection,
                   ObjectType::ekLiveObjectType)
      , DronesActivate_(false)
      , TrackingActivate_(false)
      , BulletsQuantity_(gkBulletsQuantity)
      , LastTrackingCheck_(std::chrono::steady_clock::now())
      , LastDroneActivate_(std::chrono::steady_clock::now()) {
    Coords_ = {
            {kStartCoords.first - 1, kStartCoords.second                 },
            {kStartCoords.first,     kStartCoords.second                 },
            {kStartCoords.first,     kStartCoords.second + kViewDirection},
            {kStartCoords.first + 1, kStartCoords.second                 }
    };
    Shift_ = gkStandardShift;
    NewCoords_ = Coords_;
    Avatar_ = "{~*}";
  }

  void TrackingEnemy::manageDrones(
          std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace,
          const std::chrono::time_point<std::chrono::steady_clock> kCurTime) {
    if( DronesQuantity_ > 0 ) {
      DronesQuantity_--;
      DronesActivate_ = DronesQuantity_ > 0;
      LastDroneActivate_ = kCurTime;
      trace.push_back(std::make_shared<Drone>(
              ViewDirection_, std::pair{CoreCoords_.first - 1, CoreCoords_.second + ViewDirection_},
              std::pair{0, ViewDirection_}, Fraction_, '-'));
      trace.push_back(std::make_shared<Drone>(
              ViewDirection_, std::pair{CoreCoords_.first + 1, CoreCoords_.second + ViewDirection_},
              std::pair{0, ViewDirection_}, Fraction_, '-'));
    }

    if( DronesActivate_ && !DronesQuantity_
        && std::chrono::duration_cast<std::chrono::milliseconds>(kCurTime - LastShoot_).count()
                   >= gkDronesPushTimeout ) {
      DronesActivate_ = false;
      DronesQuantity_ = gkDronesQuantity;
    }
  }

  void TrackingEnemy::manageBullets(
          std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace,
          const std::chrono::time_point<std::chrono::steady_clock> kCurTime) {
    if( BulletsQuantity_ > 0 ) {
      --BulletsQuantity_;
      LastShoot_ = kCurTime;
      trace.push_back(std::make_shared<Bullet>(
              ViewDirection_, std::pair{CoreCoords_.first, CoreCoords_.second + ViewDirection_ * 2},
              Fraction_));
    }
    else if( std::chrono::duration_cast<std::chrono::milliseconds>(kCurTime - LastShoot_).count()
             >= gkReloadTimeout ) {
      BulletsQuantity_ = gkBulletsQuantity;
    }
  }

  void TrackingEnemy::manageNextRotations(
          std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace,
          const std::chrono::time_point<std::chrono::steady_clock> kCurTime) {
    if( TrackingObject_.expired()
        || std::abs(TrackingObject_.lock()->getCoreCords().first - CoreCoords_.first)
                   > gkMaxTrackingCordsDiff ) {
      TrackingActivate_ = false;
    }

    if( std::chrono::duration_cast<std::chrono::milliseconds>(kCurTime - LastMove_).count()
        >= gkStandardMoveTimeout ) {
      RotationEnd_ = false;
      LastMove_ = kCurTime;
    }

    if( TrackingActivate_ ) {
      int diff = TrackingObject_.lock()->getCoreCords().first - CoreCoords_.first;
      diff = (diff > gkStandardShift.first) ? gkStandardShift.first * std::abs(diff) / diff : diff;
      Shift_.first = diff;
    }
    else if( !Shift_.first ) {
      Shift_.first = getRandomBoolean(0.5) ? -gkStandardShift.first : gkStandardShift.first;
    }
    else {
      Shift_.first = (getRandomBoolean(gkChangeDirChance) ? (-Shift_.first) : Shift_.first / 1)
                   * gkStandardShift.first / std::abs(Shift_.first);
    }
  }

  void TrackingEnemy::updateCondition(
          std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    // todo добавить ли сюда функцию которая будет 3 строки ниже по дефолту делать
    CoreCoords_ = NewCoreCoords_;
    Coords_ = NewCoords_;
    WeaponCond_ = (WeaponCond_ != WeaponConditions::ekNewWeapon) ? WeaponConditions::ekUnUsable
                                                                 : WeaponConditions::ekNewWeapon;
    auto curTime = std::chrono::steady_clock::now();

    manageNextRotations(trace, curTime);
    manageDrones(trace, curTime);
    manageBullets(trace, curTime);
  }

  bool TrackingEnemy::getFight(std::shared_ptr<gameObj::ShiftingObject>& enemy,
                               std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    int damage = enemy->getDamage(*this);
    if( damage > 0 && !DronesActivate_ ) {
      DronesActivate_ = true;
    }
    LivesQuantity_ -= damage;
    return isAlive();
  }

  void TrackingEnemy::interaction(std::shared_ptr<gameObj::ShiftingObject>& other,
                                  std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    getFight(other, trace);
  }

  void TrackingEnemy::action(std::vector<std::shared_ptr<gameObj::ShiftingObject>>& objects,
                             std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    for( auto& object: objects ) {
      if( (object->getFraction() != Fraction_
           || object->getFraction() == ObjectFraction::ekNoneFraction)
          && !TrackingActivate_ && object->isAlive()
          && object->getType() == ObjectType::ekLiveObjectType
          && abs(object->getCoreCords().first - CoreCoords_.first) <= gkMaxTrackingCordsDiff ) {
        TrackingActivate_ = true;
        TrackingObject_ = object;
      }
      if( this == &(*object) || !isCollision(object) ) {
        continue;
      }
      interaction(object, trace);
    }
  }

  int TrackingEnemy::sayDamage(const ShiftingObject& object) const {
    if( (object.getFraction() == Fraction_
         && object.getFraction() != ObjectFraction::ekNoneFraction)
        || WeaponCond_ == WeaponConditions::ekUnUsable ) {
      return 0;
    }

    if( Protection_ < object.sayDamage(*this) ) {
      return BattleDamage_ * gkBasicLowerCoef;
    }
    return BattleDamage_;
  }

  int TrackingEnemy::getDamage(const ShiftingObject& object) {
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

  const std::vector<std::pair<int, int>>& TrackingEnemy::offerNewCoords() {
    NewCoreCoords_.first = CoreCoords_.first + Shift_.first;
    for( int i = 0; i < NewCoords_.size(); ++i ) {
      NewCoords_[i].first = (Coords_[i].first + Shift_.first);
    }
    return NewCoords_;
  }

  bool TrackingEnemy::checkRoute(const std::vector<std::pair<bool, bool>>& allowedShift) {
    if( std::ranges::any_of(allowedShift.begin(), allowedShift.end(), [](auto& x) {
          return x != std::pair{true, true};
        }) ) {
      Shift_.first -= (Shift_.first / std::abs(Shift_.first));
      return RotationEnd_ = !Shift_.first;
    }
    return RotationEnd_ = true;
  }
}// namespace gameObj
