#ifndef BOUNDINGBOX_HPP
#define BOUNDINGBOX_HPP

#include <array>

namespace game {
class Player;
}

namespace misc {

enum class BoxCoord {
  top,     // y1
  bottom,  // y2
  left,    // x1
  right    // x2
};

class BoundingBox {
  std::array<int, 4> coords;

 public:
  // Constructor
  BoundingBox(std::array<int, 4> coords);
  // Default Constructor
  BoundingBox();

  void clear();
  bool isOutside(const BoundingBox& other) const;
  void set(BoxCoord coord, int value);
  void setCoords(int x1, int y1, int x2, int y2);
  int get(BoxCoord coord) const;
  bool visibleToPlayer(const game::Player& player) const;
};

}  // namespace misc

#endif