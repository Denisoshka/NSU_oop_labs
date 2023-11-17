#include "bullet.hpp"

namespace {
  const std::pair<int, int> bulletDirectionShift{0, 1};

  const int elapsedMSToMove = 100;
  const int gkBulletLivesQuantity = 2;
  const int gkBulletDamage = 30;
  const char gkBulletAvatar = '|';
  const int gkBulletUsesPerMove = 1;
}// namespace

namespace gameObj {
  Bullet::Bullet(const ObjDirection viewDirection, const std::pair<int, int>& startCoords,
                 const ObjectFraction fraction)
      : Weapon(viewDirection, startCoords, gkBulletAvatar, gkBulletLivesQuantity, gkBulletDamage,
               fraction, ObjectProtection::ekNoneProtection, gkBulletUsesPerMove) {
    Shift_ = bulletDirectionShift;
    if( viewDirection == ObjDirection::ekObjUp ) {
      Shift_.second = -Shift_.second;
    }
  }

  void Bullet::updateCondition(std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    CoreCoords_ = NewCoreCoords_;
    Coords_.front() = NewCoords_.front();

    auto curTime = std::chrono::steady_clock::now();
    if( std::chrono::duration_cast<std::chrono::milliseconds>(curTime - LastMoveTime_).count()
        >= elapsedMSToMove ) {
      RotationEnd_ = false;
      LastMoveTime_ = curTime;
      UsesPerMove_ = gkBulletUsesPerMove;
    }
  }

  bool Bullet::fight(ShiftingObject& object,
                     std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    if( Protection_ >= object.getProtection() && LivesQuantity_ > 0 ) {
      UsesPerMove_--;
      LivesQuantity_--;
    }
    else if( LivesQuantity_ > 0 ) {
      UsesPerMove_--;
      LivesQuantity_ = 0;
    }

    return !isAlive();
  }

  void Bullet::interaction(ShiftingObject& other,
                           std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    if(((other.getFraction() != Fraction_ || other.getFraction() == ObjectFraction::ekNoneFraction)
        && other.getType() == ekLiveObjectType ) ){
      if (UsesPerMove_> 0 && isAlive()){
        fight(other, trace);
        other.fight(*this, trace);
      }

    }
  }

  void Bullet::action(std::vector<std::shared_ptr<gameObj::ShiftingObject>>& objects,
                      std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    for( auto& object: objects ) {
      if( this == &(*object) || !isCollision(*object) ) {
        continue;
      }
      interaction(*object, trace);
    }
  }

  bool Bullet::checkRoute(const std::vector<std::pair<bool, bool>>& allowedShift) {
    if( allowedShift.front() != std::pair{true, true} ) {
      NewCoreCoords_ = CoreCoords_;
      NewCoords_.front() = Coords_.front();
      LivesQuantity_ = 0;
    }
    return RotationEnd_ = true;
  }

  const std::vector<std::pair<int, int>>& Bullet::offerNewCoords() {
    CoreCoords_.second += Shift_.second;
    NewCoords_.front().second += Shift_.second;
    return NewCoords_;
  }

}// namespace gameObj