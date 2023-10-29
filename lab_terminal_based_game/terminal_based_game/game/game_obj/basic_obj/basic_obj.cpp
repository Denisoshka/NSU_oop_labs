#include "basic_obj.hpp"

#include <utility>

namespace gameObj {
  basicObj::basicObj(std::pair<int, int>&& startCoords, char avatar)
      : Coords_(std::move(startCoords))
      , Avatar_(avatar) {
  }

  std::pair<int, int> basicObj::getCoords() {
    return Coords_;
  }

  char basicObj::avatar() const {
    return Avatar_;
  }
}// namespace gameObj
