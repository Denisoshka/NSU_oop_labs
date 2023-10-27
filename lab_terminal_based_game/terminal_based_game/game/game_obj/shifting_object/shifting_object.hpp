#pragma once
#include "basic_obj.hpp"

namespace gameObj {
  enum ObjDirection : int {
    ekObjUp = -1,
    ekOBJDown = 1,
  };

  class ShiftingObject: public basicObj {
  public:
    ShiftingObject(int Direction, std::pair<unsigned, unsigned>&& startCoords);
    virtual std::pair<unsigned, unsigned> desiredShift() const = 0;
    virtual void makeShift(std::pair<unsigned, unsigned>& allowedShift) = 0;

  protected:
    //  todo
    int Direction_;
    std::pair<unsigned, unsigned> DirectionShift_;
  };
}// namespace gameObj
