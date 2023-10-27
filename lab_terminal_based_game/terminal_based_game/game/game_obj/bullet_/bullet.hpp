#pragma once
#include <memory>
#include "shifting_object.hpp"

namespace gameObj {
  class Bullet: public ShiftingObject {
  public:
    Bullet(int DirectionDegrees, std::pair<unsigned, unsigned>&& startCoords);
    virtual std::shared_ptr<basicObj> action(const char action) override;
    virtual std::pair<unsigned, unsigned> desiredShift() const override;
    virtual void makeShift(std::pair<unsigned, unsigned>& allowedShift);

  private:
  };
}// namespace gameObj
