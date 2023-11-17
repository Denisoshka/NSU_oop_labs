#include "barricade_generator.hpp"
#include <algorithm>
#include <random>
#include "bullet.hpp"
#include "chrono"

namespace {
  const int gkBarricadeLivesQuantity = 100;
  const int gkGeneratorLivesQuantity = 300;
  const int gkGeneratorDamage = 5;
  const int gkBarricadeDamage = 10;
  const int gkGenerateBarricadeTimeout = 500;
  const int gkGeneratorMoveTimeout = 1000;
  const int gkBarricadeMoveTimeout = 700;
  const int gkBarricadeDamageLOverCoef = 4;
  const float gkChangeDirectionProbability = 0.3;

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
                       const std::pair<int, int> kShift, const ObjectFraction kFraction,
                       const char kAvatar)
      : ShiftingObject(kViewDirection, kStartCoords, kAvatar, gkGeneratorLivesQuantity,
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

    auto curTime = std::chrono::steady_clock::now();

    if( getRandomBoolean(gkChangeDirectionProbability)
        && std::chrono::duration_cast<std::chrono::milliseconds>(curTime - LastMove_).count()
                   >= gkBarricadeMoveTimeout ) {
      RotationEnd_ = false;
      LastMove_ = curTime;
      Shift_.first = getRandomBoolean(gkChangeDirectionProbability) ? -Shift_.first : Shift_.first;
    }
  }

  void Barricade::action(std::vector<std::shared_ptr<gameObj::ShiftingObject>>& objects,
                         std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    for( auto& object: objects ) {
      if( this == &(*object) || !isCollision(*object) ) {
        continue;
      }
      interaction(*object, trace);
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

  bool Barricade::fight(ShiftingObject& object,
                        std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    if( Protection_ <= object.getDamage() ) {
      Protection_ = ObjectProtection::ekNoneProtection;
      LivesQuantity_ -= object.getDamage();
    }
    else {
      Protection_ = ObjectProtection::ekBaseProtection;
      LivesQuantity_ -= object.getDamage() / gkBarricadeDamageLOverCoef;
    }
    return isAlive();
  }

  void Barricade::interaction(ShiftingObject& other,
                              std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    if( (other.getFraction() != Fraction_ || other.getFraction() == ObjectFraction::ekNoneFraction)
        && other.getType() == ekLiveObjectType) {
      other.fight(*this, trace);
    }
  }


}// namespace gameObj

namespace gameObj {
  BarricadeGenerator::BarricadeGenerator(ObjDirection kViewDirection,
                                         const std::pair<int, int>& kStartCoords,
                                         const ObjectFraction kFraction, const int kAvatar)
      : ShiftingObject(kViewDirection, kStartCoords, ' ', gkGeneratorLivesQuantity,
                       gkGeneratorDamage, kFraction, ObjectProtection::ekHighProtection,
                       ObjectType::ekLiveObjectType) {
    Shift_ = gkRightShift;
    Coords_ = {
            {kStartCoords.first - 1, kStartCoords.second},
            {kStartCoords.first,     kStartCoords.second},
            {kStartCoords.first + 1, kStartCoords.second}
    };
    NewCoords_ = Coords_;
    Avatar_ = (kFraction == ekPlayerFraction) ? "^+^" : "^-^";
  }

  void BarricadeGenerator::updateCondition(
          std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    CoreCoords_ = NewCoreCoords_;
    Coords_ = NewCoords_;

    if( LivesQuantity_ < gkBarricadeDamage ) {
      Avatar_.front() = '~';
      Avatar_.back() = '~';
    }
    Barricade obg{
            ViewDirection_, std::pair{Coords_.front().first - 1, Coords_.front().second},
            std::pair{-1,                        0                     },
             Fraction_, (Fraction_ == ekPlayerFraction) ? '+' : '-'
    };
    auto curTime = std::chrono::steady_clock::now();
    if( std::chrono::duration_cast<std::chrono::milliseconds>(curTime - LastGenerate_).count()
        >= gkGenerateBarricadeTimeout ) {
      trace.push_back(std::make_shared<Barricade>(
              ViewDirection_, std::pair{Coords_.front().first + 1, Coords_.front().second},
              std::pair{1, (Fraction_ == ekPlayerFraction) ? -1 : 1}, Fraction_,
              (Fraction_ == ekPlayerFraction) ? '+' : '-'));
      trace.push_back(std::make_shared<Barricade>(
              ViewDirection_, std::pair{Coords_.front().first + 1, Coords_.front().second},
              std::pair{1, (Fraction_ == ekPlayerFraction) ? -1 : 1}, Fraction_,
              (Fraction_ == ekPlayerFraction) ? '+' : '-'));

      LastGenerate_ = curTime;
    }

    if( std::chrono::duration_cast<std::chrono::milliseconds>(curTime - LastMove_).count()
        >= gkGeneratorMoveTimeout ) {
      Shift_.first = getRandomBoolean(gkChangeDirectionProbability) ? -Shift_.first : Shift_.first;
      RotationEnd_ = false;
      LastMove_ = curTime;
    }
  }

  void BarricadeGenerator::action(std::vector<std::shared_ptr<gameObj::ShiftingObject>>& objects,
                                  std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    for( auto& object: objects ) {
      if( this == &(*object) || !isCollision(*object) ) {
        continue;
      }
      interaction(*object, trace);
    }
  }

  const std::vector<std::pair<int, int>>& BarricadeGenerator::offerNewCoords() {
    NewCoreCoords_.first = CoreCoords_.first + Shift_.first;
    for( int i = 0; i < NewCoords_.size(); ++i ) {
      NewCoords_[i].first = (Coords_[i].first + Shift_.first);
    }
    return NewCoords_;
  }

  void BarricadeGenerator::interaction(
          ShiftingObject& other, std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    if( (other.getFraction() != Fraction_ || other.getFraction()) == ObjectFraction::ekNoneFraction
        && other.getType() == ekLiveObjectType ) {
      other.fight(*this, trace);
    }
  }

  bool BarricadeGenerator::checkRoute(const std::vector<std::pair<bool, bool>>& allowedShift) {
    if( std::ranges::any_of(allowedShift.begin(), allowedShift.end(), [](auto& x) {
          return std::pair{true, true} != x;
        }) ) {
      NewCoords_ = Coords_;
    }
    return RotationEnd_ = true;
  }

  bool BarricadeGenerator::fight(ShiftingObject& object,
                                 std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    if( Protection_ < object.getDamage() ) {
      LivesQuantity_ -= object.getDamage();
    }
    else {
      LivesQuantity_ -= object.getDamage() / 2;
    }

    return isAlive();
  }

}// namespace gameObj