#pragma once

#include <memory>
#include "vector"

namespace gameObj {
  class basicObj {
  public:
    basicObj() = default;
    basicObj(std::pair<unsigned int, unsigned int>&& startCoords);
    virtual std::shared_ptr<basicObj> action(const char action) = 0;
    virtual std::pair<unsigned int, unsigned int> getCoords();

  protected:
    std::pair<unsigned int, unsigned int> Coords_{};
  };
}// namespace gameObj
