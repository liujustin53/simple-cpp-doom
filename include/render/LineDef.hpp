#ifndef LINEDEF_HPP
#define LINEDEF_HPP

namespace render {

struct Vertex;
class SideDef;

class LineDef {
  // TODO: make private
 public:
  // Vertices, from v1 to v2
  Vertex* v1;
  Vertex* v2;

  // v2 - v1
  int dx;
  int dy;

  // Front and back sidedefs
  SideDef* sidedef[2];

 public:
  // Constructor
  LineDef(Vertex* v1, Vertex* v2, SideDef* frontSidedef,
          SideDef* backSidedef = nullptr);
};
}  // namespace render

#endif