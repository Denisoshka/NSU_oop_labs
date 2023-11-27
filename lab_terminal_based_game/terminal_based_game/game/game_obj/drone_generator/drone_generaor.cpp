#include "bullet.hpp"
#include "drone_generator.hpp"

#include <random>
#include <algorithm>
#include <chrono>

namespace {
  const int gkBarricadeLivesQuantity = 10;
  const int gkGeneratorLivesQuantity = 300;
  const int gkGeneratorDamage = 30;
  const int gkDroneDamage = 10;
  const int gkGenerateDroneReloadTimeout = 5000;
  const int gkGeneratorMoveTimeout = 7000;
  const int gkBarricadeMoveTimeout = 600;
  const int gkBarricadeDamageLOverCoef = 4;
  const int gkUsesPerMove = 1;
  const int gkDronesToGenerateQuantity = 10;

  const float gkBasicDroneLowerCoef = 0.5;
  const float gkChangeDirectionProbability = 0.3;
  const float gkBasicLowerCoef = 0.4;

  const char kBarricadeAvatar = '=';

  const std::pair<int, int> gkRightShift{1, 0};
  const std::pair<int, int> gkLeftShift{-1, 0};
  const std::pair<int, int> gkDiagRightShift{1, 0};
  const std::pair<int, int> gkDiagLeftShift{1, 0};

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
  Drone::Drone(ObjDirection kViewDirection, const std::pair<int, int>& kStartCoords,
               const std::pair<int, int>& kShift, const ObjectFraction kFraction,
               const char kAvatar)
      : LiveObject(kViewDirection, kStartCoords, kAvatar, gkBarricadeLivesQuantity, gkDroneDamage,
                   kFraction, ObjectProtection::ekNoneProtection, ObjectType::ekLiveObjectType) {
    Shift_ = kShift;
    NewCoords_ = Coords_;
  }

  void Drone::updateCondition(std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    Coords_ = NewCoords_;
    CoreCoords_ = NewCoreCoords_;

    if( LivesQuantity_ < gkBarricadeLivesQuantity / 2 ) {
      Protection_ = ObjectProtection::ekNoneProtection;
    }

    WeaponCond_ = (WeaponCond_ != WeaponConditions::ekNewWeapon) ? WeaponConditions::ekUnUsable
                                                                 : WeaponConditions::ekNewWeapon;

    auto curTime = std::chrono::steady_clock::now();

    if( getRandomBoolean(gkChangeDirectionProbability)
        && std::chrono::duration_cast<std::chrono::milliseconds>(curTime - LastMove_).count()
                   >= gkBarricadeMoveTimeout ) {
      UsesPerMove_ = gkUsesPerMove;
      RotationEnd_ = false;
      LastMove_ = curTime;
      WeaponCond_ = WeaponConditions::ekNewWeapon;
      Shift_.first = getRandomBoolean(gkChangeDirectionProbability) ? -Shift_.first : Shift_.first;
    }
  }

  const std::vector<std::pair<int, int>>& Drone::offerNewCoords() {
    NewCoreCoords_.first = CoreCoords_.first + Shift_.first;
    NewCoreCoords_.second = CoreCoords_.second + Shift_.second;
    NewCoords_.front().first = Coords_.front().first + Shift_.first;
    NewCoords_.front().second = Coords_.front().second + Shift_.second;

    return NewCoords_;
  }

  bool Drone::checkRoute(const std::vector<std::pair<bool, bool>>& allowedShift) {
    if( allowedShift.front() != std::pair{true, true} ) {
      NewCoords_ = Coords_;
      NewCoreCoords_ = CoreCoords_;
      LivesQuantity_ = 0;
    }
    return RotationEnd_ = true;
  }

  bool Drone::getFight(std::shared_ptr<gameObj::ShiftingObject>& enemy,
                       std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    if( Protection_ <= enemy->sayDamage(*this) ) {
      Protection_ = ObjectProtection::ekNoneProtection;
    }
    else {
      Protection_ = ObjectProtection::ekBaseProtection;
    }
    LivesQuantity_ -= enemy->getDamage(*this);
    return isAlive();
  }

  int Drone::getDamage(const ShiftingObject& object) {
    if( (object.getFraction() == Fraction_
         && object.getFraction() != ObjectFraction::ekNoneFraction)
        || WeaponCond_ == WeaponConditions::ekUnUsable ) {
      return 0;
    }

    WeaponCond_ = WeaponConditions::ekWasInUse;
    if( BattleDamage_ < object.getProtection() ) {
      return BattleDamage_ * gkBasicDroneLowerCoef;
    }
    return BattleDamage_;
  }

  void Drone::interaction(std::shared_ptr<gameObj::ShiftingObject>& other,
                          std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    getFight(other, trace);
  }

  int Drone::sayDamage(const ShiftingObject& object) const {
    if( (object.getFraction() == Fraction_
         && object.getFraction() != ObjectFraction::ekNoneFraction)
        || WeaponCond_ == WeaponConditions::ekUnUsable ) {
      return 0;
    }

    if( BattleDamage_ < object.getProtection() ) {
      return BattleDamage_ * gkBasicDroneLowerCoef;
    }
    return BattleDamage_;
  }

}// namespace gameObj

namespace gameObj {
  DroneGenerator::DroneGenerator(ObjDirection kViewDirection,
                                 const std::pair<int, int>& kStartCoords,
                                 const ObjectFraction kFraction, const int kAvatar)
      : LiveObject(kViewDirection, kStartCoords, ' ', gkGeneratorLivesQuantity, gkGeneratorDamage,
                   kFraction, ObjectProtection::ekHighProtection, ObjectType::ekLiveObjectType)
      , DronesQuantity_(gkDronesToGenerateQuantity) {
    Shift_ = gkRightShift;
    Coords_ = {
            {kStartCoords.first - 1, kStartCoords.second},
            {kStartCoords.first,     kStartCoords.second},
            {kStartCoords.first + 1, kStartCoords.second}
    };
    NewCoords_ = Coords_;
    Avatar_ = (kFraction == ekPlayerFraction) ? "^+^" : "^-^";
  }

  void DroneGenerator::updateCondition(
          std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    CoreCoords_ = NewCoreCoords_;
    Coords_ = NewCoords_;

    auto curTime = std::chrono::steady_clock::now();

    if( DronesQuantity_ > 0 ) {
      --DronesQuantity_;
      trace.push_back(std::make_shared<Drone>(
              ViewDirection_, std::pair{Coords_.front().first - 1, Coords_.front().second},
              std::pair{-1, (Fraction_ == ekPlayerFraction) ? -1 : 1}, Fraction_,
              (Fraction_ == ekPlayerFraction) ? '+' : '-'));
      trace.push_back(std::make_shared<Drone>(
              ViewDirection_, std::pair{Coords_.front().first + 1, Coords_.front().second},
              std::pair{1, (Fraction_ == ekPlayerFraction) ? -1 : 1}, Fraction_,
              (Fraction_ == ekPlayerFraction) ? '+' : '-'));
      LastGenerate_ = curTime;
    }
    else if( std::chrono::duration_cast<std::chrono::milliseconds>(curTime - LastGenerate_).count()
             >= gkGenerateDroneReloadTimeout ) {
      DronesQuantity_ = gkDronesToGenerateQuantity;
      LastGenerate_ = curTime;
    }

    if( std::chrono::duration_cast<std::chrono::milliseconds>(curTime - LastMove_).count()
        >= gkGeneratorMoveTimeout ) {
      Shift_.first = getRandomBoolean(gkChangeDirectionProbability) ? -Shift_.first : Shift_.first;
      RotationEnd_ = false;
      LastMove_ = curTime;
    }
  }

  const std::vector<std::pair<int, int>>& DroneGenerator::offerNewCoords() {
    NewCoreCoords_.first = CoreCoords_.first + Shift_.first;
    for( int i = 0; i < NewCoords_.size(); ++i ) {
      NewCoords_[i].first = (Coords_[i].first + Shift_.first);
    }
    return NewCoords_;
  }

  void DroneGenerator::interaction(std::shared_ptr<gameObj::ShiftingObject>& other,
                                   std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    getFight(other, trace);
  }

  bool DroneGenerator::checkRoute(const std::vector<std::pair<bool, bool>>& allowedShift) {
    if( std::ranges::any_of(allowedShift.begin(), allowedShift.end(), [](auto& x) {
          return std::pair{true, true} != x;
        }) ) {
      NewCoords_ = Coords_;
    }
    return RotationEnd_ = true;
  }

  bool DroneGenerator::getFight(std::shared_ptr<gameObj::ShiftingObject>& enemy,
                                std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    LivesQuantity_ -= enemy->getDamage(*this);
    return isAlive();
  }

  int DroneGenerator::sayDamage(const ShiftingObject& object) const {
    if( (object.getFraction() == Fraction_
         && object.getFraction() != ObjectFraction::ekNoneFraction)
        || UsesForMove_ <= 0 || WeaponCond_ == WeaponConditions::ekUnUsable ) {
      return 0;
    }

    if( Protection_ < object.sayDamage(*this) ) {
      return BattleDamage_ * gkBasicLowerCoef;
    }
    return BattleDamage_;
  }

  int DroneGenerator::getDamage(const ShiftingObject& object) {
    if( (object.getFraction() == Fraction_
         && object.getFraction() != ObjectFraction::ekNoneFraction)
        || UsesForMove_ <= 0 || WeaponCond_ == WeaponConditions::ekUnUsable ) {
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