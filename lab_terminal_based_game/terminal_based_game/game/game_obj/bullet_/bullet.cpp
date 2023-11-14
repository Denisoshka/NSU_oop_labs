#include "bullet.hpp"

namespace {
  const std::pair<int, int> bulletDirectionShift{0, 1};

  const int elapsedMSToMove = 300;
  const int gkBulletLivesQuantity = 2;
  const int gkBulletDamage = 1;
  const char gkBulletAvatar = '*';
  const int gkBulletUsesPerFrame = 1;
}// namespace

namespace gameObj {
  Bullet::Bullet(const ObjDirection viewDirection, const std::pair<int, int>& startCoords,
                 const ObjectFraction fraction)
      : Weapon(viewDirection, startCoords, gkBulletAvatar, gkBulletLivesQuantity, gkBulletDamage,
               fraction, ObjectProtection::ekNoneProtection, gkBulletUsesPerFrame) {
    Shift_ = bulletDirectionShift;
    if( viewDirection == ObjDirection::ekObjUp ) {
      Shift_.second = -Shift_.second;
    }
  }

  void Bullet::updateCondition(std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    ShiftingObject::updateCondition(trace);
    auto curTime = std::chrono::steady_clock::now();
    if( std::chrono::duration_cast<std::chrono::milliseconds>(curTime - LastMoveTime_).count()
        >= elapsedMSToMove ) {
      AbleToMove_ = true;
      LastMoveTime_ = curTime;
    }
    UsesPerFrame_ = gkBulletUsesPerFrame;
  }

  bool Bullet::fight(ShiftingObject& object,
                     std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    if( UsesPerFrame_ <= 0 ) {
    }
    else if( Protection_ >= object.getProtection() && LivesQuantity_ > 0 ) {
      UsesPerFrame_--;
      LivesQuantity_--;
    }
    else if( LivesQuantity_ > 0 ) {
      UsesPerFrame_--;
      LivesQuantity_ = 0;
    }

    return !isAlive();
  }

  void Bullet::interaction(ShiftingObject& other,
                           std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    if( (other.getFraction() != Fraction_ || other.getFraction()) == ObjectFraction::ekNoneFraction
        && other.getType() == ekLiveObjectType ) {
      fight(other, trace);
      other.fight(*this, trace);
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
      NewCoords_ = Coords_;
      LivesQuantity_ = 0;
    }
    RotationEnd_ = true;
    return RotationEnd_;
  }

  const std::vector<std::pair<int, int>>& Bullet::getNewCoords() {
    CoreCoords_.second += Shift_.second;
    NewCoords_.front().second += Shift_.second;
    return NewCoords_;
  }

}// namespace gameObj