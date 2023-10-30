#include "enemy.hpp"

namespace gameObj {
  Enemy::Enemy(int viewDirection, std::pair<int, int>&& startCoords)
      : ShiftingObject(viewDirection, std::move(startCoords), '!') {
  }

  std::pair<int, int> Enemy::desiredShift() const {
    return std::pair<int, int>{0, 0};
  }

  std::shared_ptr<ShiftingObject> Enemy::action(const int action) {
    return nullptr;
  }
}
