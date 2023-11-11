#pragma once
#include "basic_obj.hpp"

namespace gameObj {
  enum ObjDirection : int {
    ekObjUp = -1,
    ekObjNone = 0,
    ekOBJDown = 1,
  };

  class ShiftingObject: public basicObj {
  public:
    ShiftingObject() = default;
    ShiftingObject(ObjDirection viewDirection, const std::pair<int, int>& startCoords, char avatar,
                   const int livesQuantity, const int battleDamage_);
    virtual std::pair<int, int> desiredShift() const = 0;
    virtual void makeShift(std::pair<int, int>& allowedShift);
    virtual std::shared_ptr<ShiftingObject> action(const int action);
    virtual bool isAlive();
    virtual bool battle(gameObj::ShiftingObject& other);
    virtual bool getBattleDamage(gameObj::ShiftingObject& other) noexcept;
    virtual int giveBattleDamage();
  protected:
    int livesQuantity_;
    int battleDamage_;
    ObjDirection viewDirection_;
    ObjDirection moveDirection_;
    std::pair<unsigned, unsigned> directionShift_;
  };
}// namespace gameObj
