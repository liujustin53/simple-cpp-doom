#include "render/RenderState.hpp"

#include "render/Render.hpp"

namespace render {

// Deconstructor
RenderState::~RenderState() {
  for (auto& vertex : this->vertexes) {
    delete vertex;
  }
  for (auto& sector : this->sectors) {
    delete sector;
  }
  for (auto& sidedef : this->sidedefs) {
    delete sidedef;
  }
  for (auto& linedef : this->linedefs) {
    delete linedef;
  }
  for (auto& seg : this->segs) {
    delete seg;
  }
  for (auto& subsector : this->subsectors) {
    delete subsector;
  }
  for (auto& node : this->nodes) {
    delete node;
  }
}

}  // namespace render