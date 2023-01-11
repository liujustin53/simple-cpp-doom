#ifndef NODE_HPP
#define NODE_HPP

#include "misc/BoundingBox.hpp"

namespace render {

class Subsector;
class DoomRenderer;

class Node {
  // TODO: make private
 public:
  // Partition line
  int x;
  int y;
  // Distance from start to end of partition line
  int dx;
  int dy;

  // Bounding box for each child
  std::array<misc::BoundingBox, 2> bbox;

  // Child nodes
  // Represents their index in the nodes vector in RenderState
  // A negative value means it is a subsector, and the absolute value is the
  // index in the subsectors vector in RenderState
  std::array<int, 2> children;

 public:
  // Constructor
  Node(int x, int y, int dx, int dy, misc::BoundingBox rightBbox,
       misc::BoundingBox leftBbox, int rightChild, int leftChild);

  // Determines which side of the partition line the point is on
  // 0 is front, 1 is back
  int pointOnSide(int x, int y);
  void renderNode(DoomRenderer& renderer);
};

}  // namespace render

#endif