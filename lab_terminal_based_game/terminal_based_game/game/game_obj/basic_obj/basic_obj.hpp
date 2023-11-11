#pragma once

#include <memory>
#include "vector"

namespace gameObj {
  class BasicObj {
  public:
    BasicObj() = default;
    BasicObj(const std::pair<int, int>& startCoords, char avatar);
    virtual std::pair<int, int> getCoords();
    virtual char avatar() const;
    virtual bool isCollision(const BasicObj& other);

  protected:
    std::pair<int, int> Coords_{};
    char Avatar_;
  };
}// namespace gameObj
