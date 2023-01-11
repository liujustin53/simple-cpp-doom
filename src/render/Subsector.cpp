#include "render/Subsector.hpp"

#include "render/DoomRenderer.hpp"
#include "render/Sector.hpp"
#include "render/Seg.hpp"
#include "render/SideDef.hpp"
#include "game/Player.hpp"

namespace render {

Subsector::Subsector(std::vector<Seg*> segs) : segs(segs) {
  this->sector = segs[0]->sidedef->sector;
}

void Subsector::moveToSector(logic::MObj* mobj) {
  this->sector->addThing(mobj);
}

void Subsector::removeFromSector(logic::MObj* mobj) {
  this->sector->removeThing(mobj);
}

void Subsector::renderSubsector(DoomRenderer& renderer) {
  // determine if floor/ceiling are in view and update renderer with the
  // correct planes to render

  // render all the segs in the subsector
  for (auto seg : this->segs) {
    seg->renderSeg(renderer);
  }
}
}  // namespace render