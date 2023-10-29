#pragma once
#include "basic_obj.hpp"

namespace gameObj {
  enum ObjDirection : int {
    ekObjUp = -1,
    ekOBJDown = 1,
  };

  class ShiftingObject: public basicObj {
  public:
    ShiftingObject(int Direction, std::pair<int, int>&& startCoords, char avatar);
    virtual std::pair<int, int> desiredShift() const = 0;
    virtual void makeShift(std::pair<int, int>& allowedShift);
    virtual std::shared_ptr<ShiftingObject> action(const int action);

  protected:
    int Direction_;
    std::pair<unsigned, unsigned> DirectionShift_;
  };
}// namespace gameObj