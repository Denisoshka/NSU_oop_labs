#include "player.hpp"
#include "bullet.hpp"

#include <cstdint>
#include <memory>

namespace {
  const int gkMoveLeft{'a'};
  const int gkMoveRight{'d'};
  const int gkReload{'s'};
  const int gkShoot{'w'};
  const int gkMaxAmmoQuantity = 10;
  const int gkPlayerLiverQuantity = 5;
  const int gkPlayerDamage = 1;
  //  std::pair<unsigned, unsigned>
}// namespace

namespace gameObj {
  Player::Player(ObjDirection viewDirection, const std::pair<int, int>& startCoords)
      : ShiftingObject(viewDirection, startCoords, '@', gkPlayerLiverQuantity, gkPlayerDamage,
                       ObjectFraction::ekPlayerFraction, ObjectProtection::ekNoneProtection,
                       ObjectType::ekLiveObjectType) {
  }

  void Player::updateCondition(const int action,
                               std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    CoreCoords_ = NewCoreCoords_;
    Coords_.front() = NewCoords_.front();

    Shift_.first = 0;
    if( action == gkMoveLeft ) {
      Shift_.first = -1;
    }
    else if( action == gkMoveRight ) {
      Shift_.first = 1;
    }
    else if( action == gkReload ) {
      AmmoQuantity_ = gkMaxAmmoQuantity;
      //      todo make reload;
    }
    else if( action == gkShoot ) {
      if( AmmoQuantity_ ) {
        std::pair bulletCoords{CoreCoords_.first, CoreCoords_.second + ViewDirection_};
        AmmoQuantity_--;
        trace.push_back(std::make_unique<Bullet>(ViewDirection_, bulletCoords, Fraction_));
      }
    }

    RotationEnd_ = !Shift_.first;
  }

  int Player::getAmmoQuantity() const {
    return AmmoQuantity_;
  }

  void Player::interaction(ShiftingObject& other,
                           std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    if( (other.getFraction() != Fraction_ || other.getFraction()) == ObjectFraction::ekNoneFraction
        && other.getType() == ekLiveObjectType ) {
      other.fight(*this, trace);
    }
  }

  void Player::action(std::vector<std::shared_ptr<gameObj::ShiftingObject>>& objects,
                      std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    for( auto& object: objects ) {
      if( object->getNewCoords() != Coords_ ) {
        continue;
      }
      interaction(*object, trace);
    }
  }

  bool Player::fight(ShiftingObject& object,
                     std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
    if( Protection_ < object.getDamage() ) {
      LivesQuantity_ -= object.getDamage();
    }
    else {
      LivesQuantity_ -= object.getDamage() / 2;
    }
    return isAlive();
  }

  void Player::updateCondition(std::vector<std::shared_ptr<gameObj::ShiftingObject>>& trace) {
  }

  bool Player::checkRoute(const std::vector<std::pair<bool, bool>>& allowedShift) {
    if( allowedShift.front() != std::pair{true, true} ) {
      // todo нет коллизии со стеной
      NewCoreCoords_= CoreCoords_;
      NewCoords_.front()= Coords_.front();
    }

    return RotationEnd_ = true;
  }

  const std::vector<std::pair<int, int>>& Player::getNewCoords() {
    NewCoreCoords_.first += Shift_.first;
    NewCoords_.front().first += Shift_.first;
    return NewCoords_;
  }

}// namespace gameObj
