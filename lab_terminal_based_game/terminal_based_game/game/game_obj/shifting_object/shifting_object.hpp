#pragma once
#include "basic_obj.hpp"

namespace gameObj {
  enum ObjDirection : int {
    ekObjUp = -1,
    ekObjNone = 0,
    ekOBJDown = 1,
  };

  class ShiftingObject: public basicObj {
  public:
    ShiftingObject() = default;
    ShiftingObject(ObjDirection viewDirection,const std::pair<int, int>& startCoords, char avatar);
    virtual std::pair<int, int> desiredShift() const = 0;
    virtual void makeShift(std::pair<int, int>& allowedShift);
    virtual std::shared_ptr<ShiftingObject> action(const int action);

  protected:
    ObjDirection viewDirection_{};
    int moveDirection_{};
    std::pair<unsigned, unsigned> directionShift_;
  };
}// namespace gameObj
