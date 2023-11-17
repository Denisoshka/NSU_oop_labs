#include <algorithm>
#include <random>
#include "bullet.hpp"
#include "chrono"
#include "drone_generator.hpp"

namespace {
  const int gkBarricadeLivesQuantity = 10;
  const int gkGeneratorLivesQuantity = 300;
  const int gkGeneratorDamage = 30;
  const int gkBarricadeDamage = 5;
  const int gkGenerateDroneReloadTimeout = 5000;
  const int gkGeneratorMoveTimeout = 1000;
  const int gkBarricadeMoveTimeout = 700;
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
  Barricade::Barricade(ObjDirection kViewDirection, const std::pair<int, int>& kStartCoords,
                       const std::pair<int, int>& kShift, const ObjectFraction kFraction,
                       const char kAvatar)
      : LiveObject(kViewDirection, kStartCoords, kAvatar, gkBarricadeLivesQuantity,
                   gkGeneratorDamage, kFraction, ObjectProtection::ekNoneProtection,
                   ObjectType::ekLiveObjectType) {
    Shift_ = kShift;
    NewCoords_ = Coords_;
  }

  void Barricade::updateCondition(std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    Coords_ = NewCoords_;
    CoreCoords_ = NewCoreCoords_;

    if( LivesQuantity_ < gkBarricadeLivesQuantity / 2 ) {
      Protection_ = ObjectProtection::ekNoneProtection;
    }

    if( WeaponCond_ != WeaponConditions::ekNewWeapon ) {
      WeaponCond_ = WeaponConditions::ekNotUsable;
    }

    WeaponCond_ = (WeaponCond_ != WeaponConditions::ekNewWeapon) ? WeaponConditions::ekNotUsable
                                                                 : WeaponConditions::ekNewWeapon;

    auto curTime = std::chrono::steady_clock::now();

    if( getRandomBoolean(gkChangeDirectionProbability)
        && std::chrono::duration_cast<std::chrono::milliseconds>(curTime - LastMove_).count()
                   >= gkBarricadeMoveTimeout ) {
      RotationEnd_ = false;
      LastMove_ = curTime;
      WeaponCond_ = WeaponConditions::ekNewWeapon;
      Shift_.first = getRandomBoolean(gkChangeDirectionProbability) ? -Shift_.first : Shift_.first;
    }
  }

  const std::vector<std::pair<int, int>>& Barricade::offerNewCoords() {
    NewCoreCoords_.first = CoreCoords_.first + Shift_.first;
    NewCoreCoords_.second = CoreCoords_.second + Shift_.second;
    NewCoords_.front().first = Coords_.front().first + Shift_.first;
    NewCoords_.front().second = Coords_.front().second + Shift_.second;

    return NewCoords_;
  }

  bool Barricade::checkRoute(const std::vector<std::pair<bool, bool>>& allowedShift) {
    if( allowedShift.front() != std::pair{true, true} ) {
      NewCoords_ = Coords_;
      NewCoreCoords_ = CoreCoords_;
      LivesQuantity_ = 0;
    }
    return RotationEnd_ = true;
  }

  bool Barricade::getFight(ShiftingObject& object,
                           std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    if( Protection_ <= object.sayDamage(*this) ) {
      Protection_ = ObjectProtection::ekNoneProtection;
    }
    else {
      Protection_ = ObjectProtection::ekBaseProtection;
    }
    LivesQuantity_ -= object.getDamage(*this);
    return isAlive();
  }

  int Barricade::getDamage(const ShiftingObject& object) {
    if( object.getFraction() == Fraction_ || object.getFraction() != ObjectFraction::ekNoneFraction
        || UsesForMove_ <= 0 || WeaponCond_ == WeaponConditions::ekNotUsable ) {
      return 0;
    }

    if( BattleDamage_ < object.getProtection() ) {
      WeaponCond_ = WeaponConditions::ekWasInUse;
      return BattleDamage_ * gkBasicDroneLowerCoef;
    }
    else {
      WeaponCond_ = WeaponConditions::ekWasInUse;
      return BattleDamage_;
    }
  }

  void Barricade::interaction(ShiftingObject& other,
                              std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    getFight(*this, trace);
  }

  int Barricade::sayDamage(const ShiftingObject& object) const {
    if( object.getFraction() == Fraction_ || object.getFraction() != ObjectFraction::ekNoneFraction
        || UsesForMove_ <= 0 || WeaponCond_ == WeaponConditions::ekNotUsable ) {
      return 0;
    }

    if( Protection_ < object.sayDamage(*this) ) {
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

    if( LivesQuantity_ < gkBarricadeLivesQuantity / 2 ) {
      Avatar_.front() = '~';
      Avatar_.back() = '~';
    }

    Barricade obg{
            ViewDirection_, std::pair{Coords_.front().first - 1, Coords_.front().second},
            std::pair{-1,                        0                     },
             Fraction_, (Fraction_ == ekPlayerFraction) ? '+' : '-'
    };
    auto curTime = std::chrono::steady_clock::now();

    if( DronesQuantity_ > 0 ) {
      --DronesQuantity_;
      trace.push_back(std::make_shared<Barricade>(
              ViewDirection_, std::pair{Coords_.front().first - 1, Coords_.front().second},
              std::pair{-1, (Fraction_ == ekPlayerFraction) ? -1 : 1}, Fraction_,
              (Fraction_ == ekPlayerFraction) ? '+' : '-'));
      trace.push_back(std::make_shared<Barricade>(
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

  void DroneGenerator::interaction(ShiftingObject& other,
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

  bool DroneGenerator::getFight(ShiftingObject& object,
                                std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    LivesQuantity_ -= object.getDamage(*this);
    return isAlive();
  }

  int DroneGenerator::sayDamage(const ShiftingObject& object) const {
    if( object.getFraction() == Fraction_ || object.getFraction() != ObjectFraction::ekNoneFraction
        || UsesForMove_ <= 0 || WeaponCond_ == WeaponConditions::ekNotUsable ) {
      return 0;
    }

    if( Protection_ < object.sayDamage(*this) ) {
      return BattleDamage_ * gkBasicLowerCoef;
    }
    return BattleDamage_;
  }

  int DroneGenerator::getDamage(const ShiftingObject& object) {
    if( object.getFraction() == Fraction_ || object.getFraction() != ObjectFraction::ekNoneFraction
        || UsesForMove_ <= 0 || WeaponCond_ == WeaponConditions::ekNotUsable ) {
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