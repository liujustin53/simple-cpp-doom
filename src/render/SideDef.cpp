#include "render/SideDef.hpp"

namespace render {
// Constructor
SideDef::SideDef(sf::Color upper_color, sf::Color lower_color,
                 sf::Color middle_color, Sector* sector)
    : upper_color(upper_color),
      lower_color(lower_color),
      middle_color(middle_color),
      sector(sector) {}
}  // namespace render