#include "basic_obj.hpp"

#include <utility>

namespace gameObj {
  basicObj::basicObj(const std::pair<int, int>& startCoords, char avatar)
      : Coords_(startCoords)
      , Avatar_(avatar) {
  }

  std::pair<int, int> basicObj::getCoords() {
    return Coords_;
  }

  char basicObj::avatar() const {
    return Avatar_;
  }

  bool basicObj::isCollision(const basicObj& other) {
    return Coords_.first == other.Coords_.first && Coords_.second == other.Coords_.second;
  }
}// namespace gameObj
