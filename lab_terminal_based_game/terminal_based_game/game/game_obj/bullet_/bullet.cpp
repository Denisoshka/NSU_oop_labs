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
  }

  void Bullet::updateCondition(std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    auto curTime = std::chrono::steady_clock::now();
    UsesPerFrame_ = gkBulletUsesPerFrame;

    if( std::chrono::duration_cast<std::chrono::milliseconds>(curTime - LastMoveTime_).count()
        >= elapsedMSToMove ) {
      LastMoveTime_ = curTime;
      if( ViewDirection_ == ekObjUp ) {
        DirectionShift_.second = -bulletDirectionShift.second;
      }
      else if( ViewDirection_ == ekOBJDown ) {
        DirectionShift_.second = bulletDirectionShift.second;
      }
    }
    else {
      DirectionShift_.second = 0;
    }
  }

  void Bullet::interaction(ShiftingObject& other,
                           std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    if( (other.getFraction() != Fraction_ || other.getFraction()) == ObjectFraction::ekNoneFraction
        && other.getType() == ekLiveObjectType ) {
      fight(other, trace);
      other.fight(*this, trace);
    }
  }

  bool Bullet::fight(ShiftingObject& object,
                     std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    if( UsesPerFrame_ <= 0 ) {
    }
    else if( Protection_ >= object.getProtection() && LivesQuantity_ > 0 ) {
      LivesQuantity_--;
    }
    else if( LivesQuantity_ > 0 ) {
      LivesQuantity_ = 0;
    }

    return !isAlive();
  }

  void Bullet::action(std::vector<std::shared_ptr<gameObj::ShiftingObject>>& objects,
                      std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    for( auto& object: objects ) {
      if( this == &(*object) || object->getNewCoords() != Coords_ ) {
        continue;
      }
      interaction(*object, trace);
    }
  }

}// namespace gameObj