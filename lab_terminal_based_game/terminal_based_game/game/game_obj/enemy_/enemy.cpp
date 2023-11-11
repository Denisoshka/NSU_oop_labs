#include "enemy.hpp"
#include <random>
#include "bullet.hpp"

namespace {
  int elapsedTimeToShoot = 5000;
  int elapsedTimeToMove = 3000;
  float shootProbability = 0.5;
  float stayHereProbability = 0.2;

  const int gkEnemyLivesQuantity = 1;
  const int gkEnemyDamage = 1;
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
      : ShiftingObject(viewDirection, startCoords, '$', gkEnemyLivesQuantity, gkEnemyDamage)
      , LastShoot_(std::chrono::steady_clock::now())
      , LastMove_(std::chrono::steady_clock::now()) {
  }

  std::pair<int, int> Enemy::desiredShift() const {
    return DirectionShift_;
  }

  std::shared_ptr<ShiftingObject> Enemy::action(const int action) {
    auto curTime = std::chrono::steady_clock::now();
    DirectionShift_.first = 0;
    if( std::chrono::duration_cast<std::chrono::milliseconds>(curTime - LastShoot_).count()
                >= elapsedTimeToMove
        && !getRandomBoolean(stayHereProbability) ) {
      if( getRandomBoolean(0.5) ) {
        DirectionShift_.first = 1;
      }
      else {
        DirectionShift_.first = -1;
      }
      LastMove_ += (curTime - LastMove_);
    }

    if( std::chrono::duration_cast<std::chrono::milliseconds>(curTime - LastShoot_).count()
                >= elapsedTimeToShoot
        && getRandomBoolean(shootProbability) ) {
      LastShoot_ += (curTime - LastShoot_);
      std::pair bulletCoords{Coords_.first, Coords_.second + ViewDirection_};
      return std::make_unique<Bullet>(ViewDirection_, bulletCoords);
    }
    return nullptr;
  }
}// namespace gameObj
