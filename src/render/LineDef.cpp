#include "render/LineDef.hpp"

#include "render/SideDef.hpp"
#include "render/Vertex.hpp"

namespace render {
LineDef::LineDef(Vertex* v1, Vertex* v2, SideDef* frontSidedef,
                 SideDef* backSidedef)
    : v1(v1),
      v2(v2),
      dx(v2->x - v1->x),
      dy(v2->y - v1->y),
      sidedef{frontSidedef, backSidedef} {}
}  // namespace render