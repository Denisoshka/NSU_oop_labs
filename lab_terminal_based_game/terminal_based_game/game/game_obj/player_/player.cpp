#include "player.hpp"

#include <cstdint>
#include <memory>


const wchar_t kMoveLeft{L'a'};
const wchar_t kMoveRight{L'd'};
const wchar_t kReload{L's'};
const wchar_t kShoot{L's'};

std::unique_ptr<basicObj> Player::communication(const wchar_t action) {
  if( action == kMoveLeft && objCords_.second > 0UL && objCords_.second < SIZE_MAX ) {
    objCords_.second--;
  }
  else if( action == kMoveRight && objCords_.second > 0UL && objCords_.second < SIZE_MAX ) {
    objCords_.second++;
  }
  else if( action == kReload ) {
    if( reloadStart_ != 0 ) {
      // reloadStart_ = std::chrono::time;
    }
    else {
      reloadStart_ = ;
    }
  }
  else if (action == kShoot){
    return std::make_unique<>()
  }
  return nullptr;
}
