#include "basic_obj.hpp"

#include <utility>

namespace gameObj {
  basicObj::basicObj(std::pair<unsigned int, unsigned int>&& startCoords)
      : Coords_(std::move(startCoords)) {
  }

  std::pair<unsigned int, unsigned int> basicObj::getCoords() {
    return Coords_;
  }
}// namespace gameObj
