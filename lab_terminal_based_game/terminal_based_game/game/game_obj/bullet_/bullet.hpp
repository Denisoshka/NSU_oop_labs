#pragma once
#include <memory>
#include "shifting_object.hpp"

namespace gameObj {
  class Bullet: public ShiftingObject {
  public:
    Bullet(const ObjDirection viewDirection,const std::pair<int, int>& startCoords);
    virtual std::pair<int, int> desiredShift() const override;

  private:
  };
}// namespace gameObj
