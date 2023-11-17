#pragma once
#include "chrono"
#include "live_object.hpp"

namespace gameObj {

  class Barricade: public LiveObject {
  public:
    Barricade(ObjDirection kViewDirection, const std::pair<int, int>& kStartCoords,
              const std::pair<int, int>& kShift, const ObjectFraction kFraction,
              const char kAvatar);
    virtual ~Barricade() = default;
    virtual void updateCondition(
            std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
    virtual bool checkRoute(const std::vector<std::pair<bool, bool>>& allowedShift) override;
    virtual const std::vector<std::pair<int, int>>& offerNewCoords() override;
    virtual bool getFight(ShiftingObject& object,
                          std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
    virtual void interaction(ShiftingObject& other,
                             std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
    virtual int sayDamage(const ShiftingObject& object) const override;
    virtual int getDamage(const ShiftingObject& object) override;

  private:
    std::chrono::time_point<std::chrono::steady_clock> LastMove_;
    int UsesPerMove_ = 1;
  };

  class DroneGenerator: public LiveObject {
  public:
    DroneGenerator(ObjDirection kViewDirection, const std::pair<int, int>& kStartCoords,
                       const ObjectFraction kFraction, const int kAvatar);
    virtual ~DroneGenerator() = default;
    virtual void updateCondition(
            std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
    virtual bool checkRoute(const std::vector<std::pair<bool, bool>>& allowedShift) override;
    virtual const std::vector<std::pair<int, int>>& offerNewCoords() override;
    virtual bool getFight(ShiftingObject& object,
                          std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
    virtual int sayDamage(const ShiftingObject& object) const override;
    virtual int getDamage(const ShiftingObject& object) override;
    virtual void interaction(ShiftingObject& other,
                             std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;

  protected:
    int DronesQuantity_;
    std::chrono::time_point<std::chrono::steady_clock> LastGenerate_;
  };
}// namespace gameObj