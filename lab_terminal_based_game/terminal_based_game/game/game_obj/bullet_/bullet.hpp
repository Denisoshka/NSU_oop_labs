#pragma once

#include <memory>

#include "weapon.hpp"

namespace gameObj {
  class Bullet: public Weapon {
  public:
    Bullet(const ObjDirection viewDirection, const std::pair<int, int>& startCoords,
           const ObjectFraction fraction);

    virtual bool checkRoute(const std::vector<std::pair<bool, bool>>& allowedShift) override;
    virtual void action(std::vector<std::shared_ptr<gameObj::ShiftingObject>>& objects,
                        std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
    virtual void updateCondition(std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
    virtual const std::vector<std::pair<int, int>>& getNewCoords() override;
    virtual bool fight(ShiftingObject& object,
                       std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
  private:
    virtual void interaction(ShiftingObject& other,
                             std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
  };
}// namespace gameObj
