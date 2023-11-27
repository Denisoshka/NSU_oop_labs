#pragma once

#include "shifting_object.hpp"

#include <chrono>
#include <memory>
#include "bullet_destroyer.hpp"

namespace gameObj {
  class Player: public ShiftingObject {
  public:
    Player() = default;
    Player(ObjDirection viewDirection, const std::pair<int, int>& startCoords);
    virtual ~Player() = default;
    virtual void updateCondition(const int action,
                                 std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace);
    virtual void updateCondition(
            std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
    virtual int getAmmoQuantity() const;
    virtual bool checkRoute(const std::vector<std::pair<bool, bool>>& allowedShift) override;
    virtual const std::vector<std::pair<int, int>>& offerNewCoords() override;
    virtual bool getFight(std::shared_ptr<gameObj::ShiftingObject>& enemy,
                          std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
    virtual int sayDamage(const ShiftingObject& object) const override;
    virtual int getDamage(const ShiftingObject& object) override;
    virtual void interaction(std::shared_ptr<gameObj::ShiftingObject>& other,
                             std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;

  private:
    std::weak_ptr<BulletDestroyer> BulletDestroyer_;
    std::chrono::time_point<std::chrono::steady_clock> LastBulletDestroyerGenerate_;
    WeaponConditions WeaponCond_;
    int AmmoQuantity_ = 0;
    std::chrono::time_point<std::chrono::steady_clock> LastWeaponReload_;
  };
}// namespace gameObj