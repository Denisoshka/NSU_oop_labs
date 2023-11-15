#pragma once
#include "shifting_object.hpp"

#include <chrono>

namespace gameObj {
  class Enemy: public ShiftingObject {
  public:
    Enemy(ObjDirection viewDirection, const std::pair<int, int>& startCoords);
    virtual ~Enemy() = default;

    virtual bool checkRoute(const std::vector<std::pair<bool, bool>>& allowedShift) override;
    virtual void action(std::vector<std::shared_ptr<gameObj::ShiftingObject>>& objects,
                        std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
    virtual void updateCondition(
            std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
    virtual const std::vector<std::pair<int, int>>& getNewCoords() override;
    virtual bool fight(ShiftingObject& object,
                       std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;

  private:
    virtual void interaction(ShiftingObject& other,
                             std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
    int AttemptsToShift_;
    std::chrono::time_point<std::chrono::steady_clock> LastShoot_;
    std::chrono::time_point<std::chrono::steady_clock> LastMove_;
  };
}// namespace gameObj
