#include "enemy.hpp"
#include <random>
#include "bullet.hpp"

namespace {
  int elapsedTimeToShoot = 5000;
  int elapsedTimeToMove = 2000;
  float shootProbability = 0.5;
  float stayHereProbability = 0.2;
//  float changeDirProbability = 0.7;
}// namespace

static bool getRandomBoolean(double probability) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> dis(0.0, 1.0);

  double randomValue = dis(gen);

  return (randomValue < probability);
}

namespace gameObj {
  Enemy::Enemy(int viewDirection, std::pair<int, int>&& startCoords)
      : ShiftingObject(viewDirection, std::move(startCoords), '$')
      , lastShoot_(std::chrono::steady_clock::now())
      , lastMove_(std::chrono::steady_clock::now())
  {
  }

  std::pair<int, int> Enemy::desiredShift() const {
    return DirectionShift_;
  }

  std::shared_ptr<ShiftingObject> Enemy::action(const int action) {
    auto curTime = std::chrono::steady_clock::now();
    DirectionShift_.first = 0;
    if( std::chrono::duration_cast<std::chrono::milliseconds>(curTime - lastShoot_).count()
        >= elapsedTimeToMove && !getRandomBoolean(stayHereProbability)) {
      if (getRandomBoolean(0.5)){
        DirectionShift_.first = 1;
      }
      else{
        DirectionShift_.first = -1;
      }
      lastMove_ += (curTime - lastMove_);
    }

    if( std::chrono::duration_cast<std::chrono::milliseconds>(curTime - lastShoot_).count()
                >= elapsedTimeToShoot
        && getRandomBoolean(shootProbability) ) {
      lastShoot_ += (curTime - lastShoot_);
      std::pair bulletCoords{Coords_.first, Coords_.second + viewDirection_};
      return std::make_unique<Bullet>(viewDirection_, std::move(bulletCoords));
    }
    return nullptr;
  }
}// namespace gameObj
