#include "shifting_object.hpp"

namespace gameObj {
  ShiftingObject::ShiftingObject(int viewDirection, std::pair<int, int>&& startCoords, char avatar)
      : basicObj(std::move(startCoords), avatar)
      , viewDirection_(viewDirection)
      , DirectionShift_({0, 0}) {
  }

  void ShiftingObject::makeShift(std::pair<int, int>& allowedShift) {
    Coords_.first += allowedShift.first;
    Coords_.second += allowedShift.second;
  }

  std::shared_ptr<ShiftingObject> ShiftingObject::action(const int action) {
    return nullptr;
  }

}// namespace gameObj
