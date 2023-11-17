#pragma once
#include <chrono>
#include "live_object.hpp"

namespace gameObj {
  class Enemy: public LiveObject {
  public:
    Enemy(ObjDirection viewDirection, const std::pair<int, int>& startCoords);
    virtual ~Enemy() = default;

    virtual bool checkRoute(const std::vector<std::pair<bool, bool>>& allowedShift) override;
    virtual void updateCondition(
            std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
    virtual const std::vector<std::pair<int, int>>& offerNewCoords() override;
    virtual bool getFight(ShiftingObject& object,
                          std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
    virtual int sayDamage(const ShiftingObject& object) const override;
    virtual int getDamage(const ShiftingObject& object) override;

  private:
    virtual void interaction(ShiftingObject& other,
                             std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
    int AttemptsToShift_;
    std::chrono::time_point<std::chrono::steady_clock> LastShoot_;
    std::chrono::time_point<std::chrono::steady_clock> LastMove_;
  };
}// namespace gameObj
