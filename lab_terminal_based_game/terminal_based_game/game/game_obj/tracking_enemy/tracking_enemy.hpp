/*
#pragma once
#include "shifting_object.hpp"
#include <chrono>

namespace gameObj {
  class TrackingEnemy: public ShiftingObject {
  public:
    TrackingEnemy(ObjDirection viewDirection, const std::pair<int, int>& startCoords);
    virtual ~TrackingEnemy() = default;

    virtual bool checkRoute(const std::vector<std::pair<bool, bool>>& allowedShift) override;
    virtual void action(std::vector<std::shared_ptr<gameObj::ShiftingObject>>& objects,
                        std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
    virtual void updateCondition(
            std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
    virtual const std::vector<std::pair<int, int>>& offerNewCoords() override;
    virtual bool getFight(ShiftingObject& object,
                       std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;

  private:
    virtual void interaction(ShiftingObject& other,
                             std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
    int AttemptsToShift_;
    std::chrono::time_point<std::chrono::steady_clock> LastShoot_;
    std::chrono::time_point<std::chrono::steady_clock> LastMove_;
  };
}// namespace gameObj
*/
