#pragma once

#include "vector"

class basicObj {
public:
  basicObj() = default;
  basicObj(std::pair<size_t, size_t>);
  virtual void action(const wchar_t action) = 0;
  virtual std::pair<size_t, size_t> getCoords() = 0;

protected:
  std::pair<size_t, size_t> objCords_{};
  std::vector<std::pair<size_t, size_t> > coords_{};
};
