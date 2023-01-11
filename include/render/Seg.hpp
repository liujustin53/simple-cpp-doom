#ifndef SEG_HPP
#define SEG_HPP

// TODO: Remove
#include <SFML/Graphics.hpp>

namespace render {
struct Vertex;
class SideDef;
class LineDef;
class DoomRenderer;

class Seg {
  // TODO: make private
 public:
  Vertex* v1;
  Vertex* v2;

  // Line that this seg is part of
  LineDef* linedef;
  SideDef* sidedef;

 public:
  // Constructor
  Seg(Vertex* v1, Vertex* v2, LineDef* linedef, SideDef* sidedef);

  // Default constructor
  Seg() = default;

  void renderSeg(DoomRenderer& renderer);
  void renderWindowSeg(DoomRenderer& renderer);
};

}  // namespace render

#endif