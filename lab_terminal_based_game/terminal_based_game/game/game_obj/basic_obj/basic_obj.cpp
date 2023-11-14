#include "basic_obj.hpp"

#include <utility>

namespace gameObj {
  BasicObj::BasicObj(const std::pair<int, int>& startCoords, char avatar)
      : Coords_(startCoords)
      , Avatar_(avatar) {
  }

  const std::vector<std::pair<int, int>>& BasicObj::getCoords()  {
    return Coords_;
  }

  const std::vector<std::pair<int, int>>& BasicObj::getPrevCoords()  {
    return PrevCoords_;
  }

  const std::vector<char>& BasicObj::getAvatar()  {
    return Avatar_;
  }

  bool BasicObj::isCollision(const BasicObj& other) {
    return Coords_ == other.Coords_;
  }
}// namespace gameObj
