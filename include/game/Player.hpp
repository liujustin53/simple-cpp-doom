#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "logic/MObj.hpp"

namespace game {

class Player : public logic::MObj {
  // TODO: make private
 public:
  int viewz;
  int base_view_height;

 public:
  // TODO: change default values
  // Constructor
  Player(int x, int y, int z, int angle);
  void movePlayer();
};
}  // namespace game

#endif