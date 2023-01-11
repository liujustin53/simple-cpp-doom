#include "render/Node.hpp"

#include "game/Player.hpp"
#include "misc/BoundingBox.hpp"
#include "render/DoomRenderer.hpp"
#include "render/Subsector.hpp"

namespace render {

// Constructor
Node::Node(int x, int y, int dx, int dy, misc::BoundingBox rightBbox,
           misc::BoundingBox leftBbox, int rightChild, int leftChild)
    : x(x),
      y(y),
      dx(dx),
      dy(dy),
      bbox{rightBbox, leftBbox},
      children{rightChild, leftChild} {}

// Determines which side of the partition line the point is on
// 0 is front, 1 is back
int Node::pointOnSide(int x, int y) {
  // Calculate the cross product of the vector from the start of the partition
  // line to the point and the vector from the start of the partition line to
  // the end of the partition line
  int cross = (x - this->x) * this->dy - (y - this->y) * this->dx;

  // If the cross product is negative, the point is on the front side of the
  // partition line
  if (cross < 0) {
    // front side
    return 0;
  }
  // back side
  return 1;
}

// Renders all subsectors below this node
// Called on the root node
void Node::renderNode(DoomRenderer& renderer) {
  // Determine which side of the partition line the player is on
  int side =
      this->pointOnSide(renderer.state.player->x, renderer.state.player->y);

  // Determine if the side the player is on is a subsector
  if (this->children[side] < 0) {
    // Render the subsector on the side the player is on
    renderer.state.subsectors[-this->children[side] - 1]->renderSubsector(
        renderer);
  } else {
    // It is a node, so render the node
    renderer.state.nodes[this->children[side]]->renderNode(renderer);
  }

  // TODO: Implement visibleToPlayer
  // Render the node on the other side if the player can see it
  if (this->bbox[side ^ 1].visibleToPlayer(*renderer.state.player)) {
    // Determine if the other side is a subsector
    if (this->children[side ^ 1] < 0) {
      // Render the subsector on the other side
      renderer.state.subsectors[-this->children[side ^ 1] - 1]->renderSubsector(
          renderer);
    } else {
      // It is a node, so render the node
      renderer.state.nodes[this->children[side ^ 1]]->renderNode(renderer);
    }
  }
}
}  // namespace render