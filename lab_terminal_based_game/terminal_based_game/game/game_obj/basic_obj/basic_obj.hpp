#pragma once

#include <memory>
#include "vector"

namespace gameObj {
  class basicObj {
  public:
    basicObj() = default;
    basicObj(const std::pair<int, int>& startCoords, char avatar);
    virtual std::pair<int, int> getCoords();
    virtual char avatar() const;
    virtual bool isCollision(const basicObj & other);

  protected:
    std::pair<int, int> Coords_{};
    char Avatar_;
  };
}// namespace gameObj
