#pragma once
#include "chrono"
#include "shifting_object.hpp"

namespace gameObj {

  class Barricade: public ShiftingObject {
  public:
    Barricade(ObjDirection kViewDirection, const std::pair<int, int>& kStartCoords,
              const std::pair<int, int> kShift, const ObjectFraction kFraction, const char kAvatar);
    virtual ~Barricade() = default;
    virtual void updateCondition(
            std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
    virtual bool checkRoute(const std::vector<std::pair<bool, bool>>& allowedShift) override;
    virtual void action(std::vector<std::shared_ptr<gameObj::ShiftingObject>>& objects,
                        std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
    virtual const std::vector<std::pair<int, int>>& offerNewCoords() override;
    virtual bool fight(ShiftingObject& object,
                       std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
    virtual void interaction(ShiftingObject& other,
                             std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;

  private:
    std::chrono::time_point<std::chrono::steady_clock> LastMove_;
  };

  class BarricadeGenerator:public ShiftingObject {
  public:
    BarricadeGenerator(ObjDirection kViewDirection, const std::pair<int, int>& kStartCoords,
                       const ObjectFraction kFraction, const int kAvatar);
    virtual ~BarricadeGenerator() = default;
    virtual void updateCondition(
            std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
    virtual bool checkRoute(const std::vector<std::pair<bool, bool>>& allowedShift) override;
    virtual void action(std::vector<std::shared_ptr<gameObj::ShiftingObject>>& objects,
                        std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
    virtual const std::vector<std::pair<int, int>>& offerNewCoords() override;
    virtual bool fight(ShiftingObject& object,
                       std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;

  protected:
    virtual void interaction(ShiftingObject& other,
                             std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) override;
    std::chrono::time_point<std::chrono::steady_clock> LastGenerate_;
    std::chrono::time_point<std::chrono::steady_clock> LastMove_;
  };


}// namespace gameObj