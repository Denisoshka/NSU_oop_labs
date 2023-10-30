#pragma once
#include "shifting_object.hpp"

namespace gameObj {
  class Enemy: public ShiftingObject {
  public:
    Enemy(int viewDirection, std::pair<int, int>&& startCoords);
    virtual std::pair<int, int> desiredShift() const override;
    virtual std::shared_ptr<ShiftingObject> action(const int action) override;

  private:
  };
}// namespace gameObj
