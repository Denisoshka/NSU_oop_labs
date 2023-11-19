#pragma once

#include <memory>

#include "weapon.hpp"

namespace gameObj {
  class Bullet: public Weapon {
  public:
    Bullet(const ObjDirection viewDirection, const std::pair<int, int>& startCoords,
           const ObjectFraction fraction);
    virtual ~Bullet() = default;

    virtual bool getFight(std::shared_ptr<gameObj::ShiftingObject>& enemy,
                          std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
    virtual bool checkRoute(const std::vector<std::pair<bool, bool>>& allowedShift) override;
    virtual void updateCondition(
            std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
    virtual const std::vector<std::pair<int, int>>& offerNewCoords() override;

    virtual int sayDamage(const ShiftingObject& object) const override;
    virtual int getDamage(const ShiftingObject& object) override;
    virtual void interaction(std::shared_ptr<gameObj::ShiftingObject>& other,
                             std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;

  protected:
  };
}// namespace gameObj
