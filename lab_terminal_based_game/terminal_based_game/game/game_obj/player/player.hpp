#pragma once
#include <chrono>
#include <memory>
#include "shifting_object.hpp"

namespace gameObj {
  class Player: public ShiftingObject {
  public:
    Player() = default;
    Player( int DirectionDegrees, std::pair< int,  int>&& startCoords);
    virtual std::shared_ptr<ShiftingObject> action(const int action) override;
    virtual std::pair<int, int> desiredShift() const override;
  private:
    size_t ammoQuantity_ = 0;
    std::chrono::seconds reloadStart_;
  };
}// namespace game_obj