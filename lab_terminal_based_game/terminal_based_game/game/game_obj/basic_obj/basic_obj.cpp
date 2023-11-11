#include "basic_obj.hpp"

#include <utility>

namespace gameObj {
  BasicObj::BasicObj(const std::pair<int, int>& startCoords, char avatar)
      : Coords_(startCoords)
      , Avatar_(avatar) {
  }

  std::pair<int, int> BasicObj::getCoords() {
    return Coords_;
  }

  char BasicObj::avatar() const {
    return Avatar_;
  }

  bool BasicObj::isCollision(const BasicObj& other) {
    return Coords_.first == other.Coords_.first && Coords_.second == other.Coords_.second;
  }
}// namespace gameObj
