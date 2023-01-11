#ifndef SUBSECTOR_HPP
#define SUBSECTOR_HPP

#include <vector>

namespace logic {
class MObj;
}

namespace render {

class Sector;
class Seg;
class DoomRenderer;

class Subsector {
  // TODO: make private
 public:
  // Sector this subsector is in
  Sector* sector;
  // Segs making up this subsector
  std::vector<Seg*> segs;

 public:
  // Constructor
  Subsector(std::vector<Seg*> segs);

  // move map object to the sector this subsector is in
  void moveToSector(logic::MObj* mobj);

  // remove map object from the sector this subsector is in
  void removeFromSector(logic::MObj* mobj);

  void renderSubsector(DoomRenderer& renderer);
};

}  // namespace render

#endif