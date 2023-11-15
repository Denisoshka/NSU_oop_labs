#pragma once

#include <memory>
#include "vector"

namespace gameObj {
  class BasicObj {
  public:
    BasicObj() = default;
    BasicObj(const std::pair<int, int>& startCoords, char avatar);
    bool isCollision(const BasicObj& other);
    virtual const std::vector<char>& getAvatar();
    virtual const std::vector<std::pair<int, int>>& getCoords();

  protected:
    const std::vector<char> Avatar_;
    std::pair<int, int> CoreCoords_;
    std::vector<std::pair<int, int>> Coords_;
  };
}// namespace gameObj
