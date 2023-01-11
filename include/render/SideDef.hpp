#ifndef SIDEDEF_HPP
#define SIDEDEF_HPP

#include <SFML/Graphics.hpp>

namespace render {

class Sector;

class SideDef {
  // TODO: make private
 public:
  // Colors for upper wall, bottom wall, and middle wall
  sf::Color upper_color;
  sf::Color lower_color;
  sf::Color middle_color;

  // Sector this sidedef is facing
  Sector* sector;

 public:
  // Constructor
  SideDef(sf::Color upper_color, sf::Color lower_color, sf::Color middle_color,
          Sector* sector);
};
}  // namespace render

#endif