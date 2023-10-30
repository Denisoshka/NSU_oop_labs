#pragma once

#include <memory>
#include "vector"

namespace gameObj {
  class basicObj {
  public:
    basicObj() = default;
    basicObj(std::pair<int, int>&& startCoords, char avatar);
    //    virtual std::shared_ptr<basicObj> action(const char action) = 0;
    virtual std::pair<int, int> getCoords();
    virtual char avatar() const;

  protected:
    std::pair<int, int> Coords_{};
    char Avatar_;
  };
}// namespace gameObj
