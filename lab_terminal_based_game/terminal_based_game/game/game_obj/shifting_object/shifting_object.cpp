#include "shifting_object.hpp"

namespace gameObj {
  ShiftingObject::ShiftingObject(int Direction,
                                 std::pair<unsigned int, unsigned int>&& startCoords)
      : basicObj(std::move(startCoords)) {
  }
}// namespace game_obj
