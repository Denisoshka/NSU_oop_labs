#pragma once

#include <memory>
#include "vector"

namespace gameObj {
  class BasicObj {
  public:
    BasicObj() = default;
    BasicObj(const std::pair<int, int>& startCoords, char avatar);
    virtual bool isCollision(const BasicObj& other);
    virtual const std::vector<char>& getAvatar() const;
    virtual const std::vector<std::pair<int, int>>& getCoords() const;

  protected:
    const std::vector<char> Avatar_;
    std::pair<int, int> CoreCoords_;
    std::vector<std::pair<int, int>> Coords_;
  };
}// namespace gameObj
