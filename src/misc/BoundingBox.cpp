#include "misc/BoundingBox.hpp"

#include <array>
#include <limits>

#include "game/Player.hpp"

namespace misc {
BoundingBox::BoundingBox(std::array<int, 4> coords) : coords(coords) {}

BoundingBox::BoundingBox() { this->clear(); }

void BoundingBox::clear() {
  set(BoxCoord::top, std::numeric_limits<int>::min());
  set(BoxCoord::right, std::numeric_limits<int>::min());
  set(BoxCoord::bottom, std::numeric_limits<int>::max());
  set(BoxCoord::left, std::numeric_limits<int>::max());
}

bool BoundingBox::isOutside(const BoundingBox& other) const {
  // Check if the box is outside the other box
  return (get(BoxCoord::left) >= other.get(BoxCoord::right) ||
          get(BoxCoord::right) <= other.get(BoxCoord::left) ||
          get(BoxCoord::top) <= other.get(BoxCoord::bottom) ||
          get(BoxCoord::bottom) >= other.get(BoxCoord::top));
}

void BoundingBox::set(BoxCoord coord, int value) {
  this->coords[static_cast<int>(coord)] = value;
}

void BoundingBox::setCoords(int x1, int y1, int x2, int y2) {
  // Check the coordinates and set
  if (x1 < x2) {
    set(BoxCoord::left, x1);
    set(BoxCoord::right, x2);
  } else {
    set(BoxCoord::left, x2);
    set(BoxCoord::right, x1);
  }

  if (y1 < y2) {
    set(BoxCoord::top, y2);
    set(BoxCoord::bottom, y1);
  } else {
    set(BoxCoord::top, y1);
    set(BoxCoord::bottom, y2);
  }
}

int BoundingBox::get(BoxCoord coord) const {
  return this->coords[static_cast<int>(coord)];
}

// TODO: implement, doesn't actually work
// Returns true is some part of the bbox is visible to the player
// Note: I don't know how this works, check source code
int checkcoord[12][4] = {{3, 0, 2, 1}, {3, 0, 2, 0}, {3, 1, 2, 0}, {0},
                         {2, 0, 2, 1}, {0, 0, 0, 0}, {3, 1, 3, 0}, {0},
                         {2, 0, 3, 1}, {2, 1, 3, 1}, {2, 1, 3, 0}};

bool BoundingBox::visibleToPlayer(const game::Player& player) const {
  int boxx;
  int boxy;
  int boxpos;

  int angle1;
  int angle2;
  int span;
  int tspan;

  int sx1;
  int sx2;

  // Find the corners of the box
  // that define the edges from current viewpoint.
  if (player.x <= this->get(BoxCoord::left))
    boxx = 0;
  else if (player.x < this->get(BoxCoord::right))
    boxx = 1;
  else
    boxx = 2;

  if (player.y >= this->get(BoxCoord::top))
    boxy = 0;
  else if (player.y > this->get(BoxCoord::bottom))
    boxy = 1;
  else
    boxy = 2;

  boxpos = (boxy << 2) + boxx;

  // Player is inside the box
  if (boxpos == 5) return true;

  const int x1 = this->get(static_cast<BoxCoord>(checkcoord[boxpos][0]));
  const int y1 = this->get(static_cast<BoxCoord>(checkcoord[boxpos][1]));
  const int x2 = this->get(static_cast<BoxCoord>(checkcoord[boxpos][2]));
  const int y2 = this->get(static_cast<BoxCoord>(checkcoord[boxpos][3]));

  return true;
}
}  // namespace misc