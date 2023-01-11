#include "logic/MObj.hpp"

#include "render/Subsector.hpp"

namespace logic {
MObj::MObj(int x, int y, int z, int angle) : x(x), y(y), z(z), angle(angle) {}

void MObj::moveToSubsector(render::Subsector* subsector) {
  if (this->subsector != nullptr) {
    this->subsector->removeFromSector(this);
  }
  this->subsector = subsector;
  this->subsector->moveToSector(this);
}

render::Subsector* MObj::getSubsector() { return this->subsector; }
}  // namespace logic