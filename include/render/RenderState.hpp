#ifndef RENDERSTATE_HPP
#define RENDERSTATE_HPP

#include <vector>

namespace game {
class Player;
}

namespace render {

class Vertex;
class Seg;
class Sector;
class Subsector;
class Node;
class LineDef;
class SideDef;

struct RenderState {
  // Rendering Data
  int view_width;
  int view_height;
  float pixel_scale;

  // Map Data
  std::vector<Vertex*> vertexes;
  std::vector<Sector*> sectors;
  std::vector<SideDef*> sidedefs;
  std::vector<LineDef*> linedefs;
  std::vector<Seg*> segs;
  std::vector<Subsector*> subsectors;
  std::vector<Node*> nodes;

  // Player
  game::Player* player;

  // Default Constructor
  RenderState() = default;
  // Deconstructor
  ~RenderState();
};

}  // namespace render

#endif