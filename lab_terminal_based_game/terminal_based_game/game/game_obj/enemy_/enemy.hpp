#pragma once
#include "shifting_object.hpp"

#include <chrono>

namespace gameObj {
  class Enemy: public ShiftingObject {
  public:
    Enemy(ObjDirection viewDirection, const std::pair<int, int>& startCoords);
    virtual ~Enemy() = default;
    virtual std::pair<int, int> desiredShift() const override;
    virtual std::shared_ptr<ShiftingObject> action(const int action) override;

  private:
    std::chrono::time_point<std::chrono::steady_clock> LastShoot_;
    std::chrono::time_point<std::chrono::steady_clock> LastMove_;
  };
}// namespace gameObj
