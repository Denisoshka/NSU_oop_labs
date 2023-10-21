#pragma once
#include "basic_obj.hpp"

#include <chrono>
#include <memory>

class Player : public basicObj{
public:
  Player() = default;
  virtual void action(const wchar_t action) override;
  std::unique_ptr<basicObj> communication(wchar_t action);
  virtual std::pair<size_t, size_t> getCoords() override;
private:
//  std::pair<size_t,size_t > Coords_;
  size_t ammoQuantity_ = 0;
  std::chrono::seconds reloadStart_;


};
