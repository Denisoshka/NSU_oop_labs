#pragma once

#include "drone_generator.hpp"
#include "live_object.hpp"

#include <chrono>

namespace gameObj {
  class TrackingEnemy: public LiveObject {
  public:
    TrackingEnemy(ObjDirection kViewDirection, const std::pair<int, int>& kStartCoords);
    virtual ~TrackingEnemy() = default;

    virtual bool checkRoute(const std::vector<std::pair<bool, bool>>& allowedShift) override;
    virtual void action(std::vector<std::shared_ptr<gameObj::ShiftingObject>>& objects,
                        std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
    virtual void updateCondition(
            std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
    virtual const std::vector<std::pair<int, int>>& offerNewCoords() override;
    virtual bool getFight(std::shared_ptr<gameObj::ShiftingObject>& enemy,
                          std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
    virtual void interaction(std::shared_ptr<gameObj::ShiftingObject>& other,
                             std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
    virtual int sayDamage(const ShiftingObject& object) const override;
    virtual int getDamage(const ShiftingObject& object) override;
    /*virtual void action(std::vector<std::shared_ptr<gameObj::ShiftingObject>>& objects,
           std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;*/
  private:
    virtual void manageDrones(std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace, const std::chrono::time_point<std::chrono::steady_clock> kCurTime);
    virtual void manageBullets(std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace, const std::chrono::time_point<std::chrono::steady_clock> kCurTime);
    virtual void manageNextRotations(std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace, const std::chrono::time_point<std::chrono::steady_clock> kCurTime);
    bool DronesActivate_;
    bool TrackingActivate_;

    int DronesQuantity_;
    int BulletsQuantity_;
    int ShiftSign_;

    std::weak_ptr<gameObj::ShiftingObject> TrackingObject_;

    std::chrono::time_point<std::chrono::steady_clock> LastTrackingCheck_;
    std::chrono::time_point<std::chrono::steady_clock> LastDroneActivate_;
  };
}// namespace gameObj
