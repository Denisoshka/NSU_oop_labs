#pragma once
#include <chrono>
#include <memory>
#include "shifting_object.hpp"

namespace gameObj {
  class Player: public ShiftingObject {
  public:
    Player() = default;
    Player(unsigned int DirectionDegrees, std::pair<unsigned int, unsigned int>&& startCoords);

    virtual std::shared_ptr<basicObj> action(const char action) override;

  private:
    //  std::pair<size_t,size_t > Coords_;
    size_t ammoQuantity_ = 0;
    std::chrono::seconds reloadStart_;
  };
}// namespace game_obj