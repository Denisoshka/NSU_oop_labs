#pragma once
#include <chrono>
#include <memory>
#include "shifting_object.hpp"

namespace gameObj {
  class Player: public ShiftingObject {
  public:
    Player() = default;
    Player(ObjDirection viewDirection, const std::pair<int, int>& startCoords);

    virtual void updateCondition(const int action,
                                 std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace);
    virtual void updateCondition(std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
    virtual std::pair<int, int> desiredShift() const override;
    virtual int getAmmoQuantity() const;
    virtual void action(std::vector<std::shared_ptr<gameObj::ShiftingObject>>& objects,
                std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
    virtual bool fight(ShiftingObject& object,
                       std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
//    virtual void updateCondition(std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
  private:
    virtual void interaction(ShiftingObject& other,
                             std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
    int AmmoQuantity_ = 0;
    std::chrono::seconds ReloadStart_{};
  };
}// namespace gameObj