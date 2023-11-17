#include "enemy.hpp"
#include <random>
#include "bullet.hpp"

namespace {
  const int elapsedMSToShoot = 700;
  const int elapsedMSToMove = 1000;
  const int gkEnemyLivesQuantity = 50;
  const int gkEnemyDamage = 30;
  const int gkBasicXShift = 1;
  const int gkAttemptsToShift = 2;

  const float shootProbability = 0.5;
  const float stayHereProbability = 0.2;
  const float gkChangeDirectionProbability = 0.3;

  const char gkEnemyAvatar = 'W';

  const std::pair<int, int> gkEnemyShift{1, 0};
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
  Enemy::Enemy(ObjDirection viewDirection, const std::pair<int, int>& startCoords)
      : ShiftingObject(viewDirection, startCoords, gkEnemyAvatar, gkEnemyLivesQuantity,
                       gkEnemyDamage, ObjectFraction::ekEnemyFraction,
                       ObjectProtection::ekNoneProtection, ObjectType::ekLiveObjectType)
      , LastShoot_(std::chrono::steady_clock::now())
      , LastMove_(std::chrono::steady_clock::now())
      , AttemptsToShift_(gkAttemptsToShift) {
    Shift_ = gkEnemyShift;
  }

  void Enemy::updateCondition(std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    CoreCoords_ = NewCoreCoords_;
    Coords_.front() = NewCoords_.front();
    AttemptsToShift_ = gkAttemptsToShift;

    srandom(time(nullptr));
    auto curTime = std::chrono::steady_clock::now();

    if( getRandomBoolean(gkChangeDirectionProbability)
        && std::chrono::duration_cast<std::chrono::milliseconds>(curTime - LastMove_).count()
                   >= elapsedMSToMove ) {
      RotationEnd_ = false;
      LastMove_ = curTime;
      Shift_.first = getRandomBoolean(gkChangeDirectionProbability) ? -Shift_.first : Shift_.first;
    }

    if( std::chrono::duration_cast<std::chrono::milliseconds>(curTime - LastShoot_).count()
                >= elapsedMSToShoot
        && getRandomBoolean(shootProbability) ) {
      LastShoot_ = curTime;
      trace.push_back(std::make_shared<Bullet>(
              ViewDirection_, std::pair{CoreCoords_.first, CoreCoords_.second + ViewDirection_},
              Fraction_));

    }
  }

  void Enemy::action(std::vector<std::shared_ptr<gameObj::ShiftingObject>>& objects,
                     std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    for( auto& object: objects ) {
      if( this == &(*object) || !isCollision(*object) ) {
        continue;
      }
      interaction(*object, trace);
    }
  }

  bool Enemy::fight(ShiftingObject& object,
                    std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    if( Protection_ < object.getDamage() ) {
      LivesQuantity_ -= object.getDamage();
    }
    else {
      LivesQuantity_ -= object.getDamage() / 2;
    }

    return isAlive();
  }

  bool Enemy::checkRoute(const std::vector<std::pair<bool, bool>>& allowedShift) {
    if( allowedShift.front() != std::pair{true, true} ) {
      NewCoords_.front().first += -Shift_.first;
      NewCoreCoords_.first += -Shift_.first;
    }
    else {
      RotationEnd_ = true;
    }

    if( AttemptsToShift_ <= 0 && !RotationEnd_ ) {
      RotationEnd_ = true;
      NewCoords_ = Coords_;
      NewCoreCoords_ = CoreCoords_;
    }

    return RotationEnd_;
  }

  const std::vector<std::pair<int, int>>& Enemy::offerNewCoords() {
    --AttemptsToShift_;
    NewCoreCoords_.first = CoreCoords_.first + Shift_.first;
    NewCoords_.front().first = Coords_.front().first + Shift_.first;
    return NewCoords_;
  }

  void Enemy::interaction(ShiftingObject& other,
                          std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    if( (other.getFraction() != Fraction_ || other.getFraction()) == ObjectFraction::ekNoneFraction
        && other.getType() == ekLiveObjectType ) {
      fight(other, trace);
      other.fight(*this, trace);
    }
  }

}// namespace gameObj
