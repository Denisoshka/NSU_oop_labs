#include "basic_obj.hpp"

#include <algorithm>
#include <utility>

namespace gameObj {
  BasicObj::BasicObj(const std::pair<int, int>& startCoords, char avatar)
      : Avatar_(std::vector{avatar})
      , CoreCoords_(startCoords)
      , Coords_(std::vector{startCoords}) {
  }

  const std::vector<std::pair<int, int>>& BasicObj::getCoords() {
    return Coords_;
  }

  const std::vector<char>& BasicObj::getAvatar() {
    return Avatar_;
  }

  bool BasicObj::isCollision(const BasicObj& other) {
    bool flag = CoreCoords_ == other.CoreCoords_;
    for( auto cords = Coords_.cbegin(); cords != Coords_.end() && !flag; ++cords ) {
      flag = std::ranges::any_of(other.Coords_.begin(), other.Coords_.end(),
                                 [&cords](auto& x) { return x == *cords; });
    }
    return flag;
  }
}// namespace gameObj
