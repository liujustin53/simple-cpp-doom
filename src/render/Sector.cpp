#include "render/Sector.hpp"

#include "logic/MObj.hpp"

namespace render {
// Constructor
Sector::Sector(int floor_height, int ceiling_height, sf::Color floor_color,
               sf::Color ceiling_color, std::array<int, 5> light_levels)
    : floor_height(floor_height),
      ceiling_height(ceiling_height),
      floor_color(floor_color),
      ceiling_color(ceiling_color),
      light_levels(light_levels) {}  // namespace render

int Sector::getLightLevel(SectorLightType type) {
  return light_levels[static_cast<int>(type)];
}

void Sector::addThing(logic::MObj* thing) { things.push_back(thing); }

void Sector::removeThing(logic::MObj* thing) {
  // if the thing is not found, throw an exception
  if (std::find(things.begin(), things.end(), thing) == things.end()) {
    throw std::invalid_argument("Thing not found.");
  }
  // remove the thing
  things.erase(std::remove(things.begin(), things.end(), thing), things.end());
}

void Sector::addLineDef(LineDef* linedef) { linedefs.push_back(linedef); }

}  // namespace render