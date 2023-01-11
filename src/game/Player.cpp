#include "game/Player.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>

#include "misc/Utils.hpp"


namespace game {

Player::Player(int x, int y, int z, int angle)
    : MObj(x, y, z, angle), viewz(z + 40), base_view_height(viewz){};

void Player::movePlayer() {
  // Turn Right
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
    this->angle -= 3;
  }
  // Turn Left
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
    this->angle += 3;
  }
  // Normalize angle
  this->angle = this->angle % 360;

  // TODO: figure out how to implement with collision
  const float rad = misc::degToRad(this->angle);
  const int dx = std::sin(rad) * 4.0;
  const int dy = std::cos(rad) * 4.0;
  // Move Forward
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
    this->x += dx;
    this->y += dy;
  }
  // Move Backward
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
    this->x -= dx;
    this->y -= dy;
  }
  // Strafe Left
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
    this->x += dy;
    this->y -= dx;
  }
  // Strafe Right
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
    this->x -= dy;
    this->y += dx;
  }
}

}  // namespace game