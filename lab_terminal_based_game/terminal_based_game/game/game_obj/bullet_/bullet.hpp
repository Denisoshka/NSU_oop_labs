#pragma once
#include <memory>
#include "shifting_object.hpp"

namespace gameObj {
  class Bullet: public ShiftingObject {
  public:
    Bullet(const ObjDirection viewDirection,const std::pair<int, int>& startCoords);
//    virtual std::shared_ptr<ShiftingObject> action(const int action) override;
    virtual std::pair<int, int> desiredShift() const override;
//    virtual void makeShift(std::pair<unsigned, unsigned>& allowedShift) override;

  private:
  };
}// namespace gameObj
