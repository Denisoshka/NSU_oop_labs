#include "enemy.hpp"
#include <random>
#include "bullet.hpp"

namespace {
  const int elapsedMSToShoot = 5000;
  const int elapsedMSToMove = 3000;
  const int gkEnemyLivesQuantity = 1;
  const int gkEnemyDamage = 1;
  const int gkBasicXShift = 1;

  const float shootProbability = 0.5;
  const float stayHereProbability = 0.2;
  const float gkChangeDirectionProbability = 0.3;

  const char gkEnemyAvatar = '@';
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
      : ShiftingObject(viewDirection, startCoords, '$', gkEnemyLivesQuantity, gkEnemyDamage,
                       ObjectFraction::ekEnemyFraction, ObjectProtection::ekNoneProtection,
                       ObjectType::ekLiveObjectType)
      , LastShoot_(std::chrono::steady_clock::now())
      , LastMove_(std::chrono::steady_clock::now()) {
  }

  std::pair<int, int> Enemy::desiredShift() const {
    return DirectionShift_;
  }

  void Enemy::updateCondition(std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    srandom(time(nullptr));
    auto curTime = std::chrono::steady_clock::now();

    if( getRandomBoolean(stayHereProbability) ) {
      DirectionShift_.first = 0;
    }
    else if( getRandomBoolean(gkChangeDirectionProbability)
             && std::chrono::duration_cast<std::chrono::milliseconds>(curTime - LastShoot_).count()
                        >= elapsedMSToMove ) {
      LastMove_ = curTime;
      if( !DirectionShift_.first ) {
        DirectionShift_.first = getRandomBoolean(0.5) ? gkBasicXShift : -gkBasicXShift;
      }
      else {
        DirectionShift_.first = -DirectionShift_.first;
      }
    }

    if( std::chrono::duration_cast<std::chrono::milliseconds>(curTime - LastShoot_).count()
                >= elapsedMSToShoot
        && getRandomBoolean(shootProbability) ) {
      LastShoot_ = curTime;
      trace.push_back(std::make_unique<Bullet>(
              ViewDirection_, std::pair{Coords_.first, Coords_.second + ViewDirection_},
              Fraction_));
    }
  }

  void Enemy::action(std::vector<std::shared_ptr<gameObj::ShiftingObject>>& objects,
                     std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    for( auto& object: objects ) {
      if( this == &(*object) || object->getCoords() != Coords_ ) {
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

    return isAlive();  }

  void Enemy::interaction(ShiftingObject& other,
                          std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    if( (other.getFraction() != Fraction_ || other.getFraction()) == ObjectFraction::ekNoneFraction
        && other.getType() == ekLiveObjectType ) {
      fight(other, trace);
      other.fight(*this, trace);
    }
  }

}// namespace gameObj
