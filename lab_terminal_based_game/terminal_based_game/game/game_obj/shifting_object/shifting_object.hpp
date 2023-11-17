#pragma once
#include <memory>
#include <utility>
#include <vector>

namespace gameObj {
  enum ObjDirection : int {
    ekObjUp = -1,
    ekObjNone = 0,
    ekOBJDown = 1,
  };

  enum ObjectFraction : int {
    ekNoneFraction = 0,
    ekPlayerFraction = 100,
    ekPeacefulFraction = 200,
    ekEnemyFraction = 300,
  };

  enum ObjectProtection : int {
    ekNoneProtection = 0,
    ekBaseProtection = 100,
    ekStandardProtection = 200,
    ekHighProtection = 300,
  };

  enum ObjectType : int {
    ekNoneType = 0,
    ekLiveObjectType = 100,
    ekWeaponType = 200,
  };

  class ShiftingObject{
  public:
    ShiftingObject() = default;
    ShiftingObject(ObjDirection kViewDirection, const std::pair<int, int>& kStartCoords,
                   char kAvatar, const int kLivesQuantity, const int kBattleDamage,
                   const ObjectFraction kFraction, const ObjectProtection kProtection_,
                   const ObjectType kType);
    virtual const std::string& getAvatar();
    virtual bool isCollision(const ShiftingObject& other);
    virtual bool checkRoute(const std::vector<std::pair<bool, bool>>& allowedShift) = 0;
    virtual void action(std::vector<std::shared_ptr<gameObj::ShiftingObject>>& objects,
                        std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) = 0;
    virtual const std::vector<std::pair<int, int>>& offerNewCoords( ) = 0;
    virtual bool fight(ShiftingObject& object,
                       std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) = 0;
    virtual void updateCondition(std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace)  = 0;

//    virtual const std::vector<std::pair<int, int>>& getCoords() const ;
//    virtual const std::pair<int, int>& getCoreCoords() const ;

    virtual bool rotationEnd() const;
    virtual const std::vector<std::pair<int, int>>&  getCoords();
    virtual const std::vector<std::pair<int, int>>& getNewCoords();
    virtual ObjectProtection getProtection() const;
    virtual ObjectFraction getFraction() const;
    virtual int getDamage() const;
    virtual int getLivesQuantity() const;
    virtual ObjectType getType() const;
    virtual bool isAlive() const;

  protected:
    virtual void interaction(ShiftingObject& other,
                             std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) = 0;
    ObjectFraction Fraction_;
    ObjectProtection Protection_;
    ObjectType Type_;

    bool RotationEnd_;

    int LivesQuantity_;
    int BattleDamage_;

    ObjDirection ViewDirection_;
    ObjDirection MoveDirection_;

    std::pair<int, int> Shift_;

    std::string Avatar_;

///    must update only in updateCondition,
    std::pair<int, int> CoreCoords_;
    std::vector<std::pair<int, int>> Coords_;

/// use for check collisions and etc
    std::pair<int, int> NewCoreCoords_;
    std::vector<std::pair<int, int>> NewCoords_;
  };
}// namespace gameObj
