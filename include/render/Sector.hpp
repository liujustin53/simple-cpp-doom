#ifndef SECTOR_HPP
#define SECTOR_HPP

#include <SFML/Graphics.hpp>
#include <vector>

namespace logic {
class MObj;
}

namespace render {

class LineDef;

enum class SectorLightType { floor, ceiling, thing, wall_top, wall_bottom };

class Sector {
  // Brightness values for floor, ceiling, light, wall top, wall bottom
  // -255 to 255, 0 being darkest, 255 being normal brightness
  std::array<int, 5> light_levels;

  // Data later inited
  // Things in this sector
  std::vector<logic::MObj*> things;

  // TODO: maybe delete?
  // LineDefs in this sector
  std::vector<LineDef*> linedefs;

 public:
  int floor_height;
  int ceiling_height;
  sf::Color floor_color;
  sf::Color ceiling_color;

  // Constructor
  Sector(int floor_height, int ceiling_height, sf::Color floor_color,
         sf::Color ceiling_color, std::array<int, 5> light_levels);

  // Get the light level for the given light type
  int getLightLevel(SectorLightType type);

  // Add a map object to this sector
  void addThing(logic::MObj* thing);

  // Remove a map object from this sector
  void removeThing(logic::MObj* thing);

  // Add a LineDef to this sector
  void addLineDef(LineDef* linedef);
};
}  // namespace render

#endif
