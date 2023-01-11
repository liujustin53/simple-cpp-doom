#include "render/Seg.hpp"

#include <cmath>
#include <iostream>

#include "game/Player.hpp"
#include "misc/Utils.hpp"
#include "render/DoomRenderer.hpp"
#include "render/LineDef.hpp"
#include "render/Sector.hpp"
#include "render/SideDef.hpp"
#include "render/Vertex.hpp"

namespace render {

// Constructor
Seg::Seg(Vertex* v1, Vertex* v2, LineDef* linedef, SideDef* sidedef)
    : v1(v1), v2(v2), linedef(linedef), sidedef(sidedef){};

void Seg::renderSeg(DoomRenderer& renderer) {
  // player angle values
  const float rad = misc::degToRad(renderer.state.player->angle);
  const float sn = std::sin(rad);
  const float cs = std::cos(rad);

  // positions relative to player
  int x1 = v1->x - renderer.state.player->x;
  int y1 = v1->y - renderer.state.player->y;
  int x2 = v2->x - renderer.state.player->x;
  int y2 = v2->y - renderer.state.player->y;

  // rotate to player angle
  int rx1 = x1 * cs - y1 * sn;
  int rx2 = x2 * cs - y2 * sn;
  int ry1 = x1 * sn + y1 * cs;
  int ry2 = x2 * sn + y2 * cs;

  // bottom of wall
  int bot = this->sidedef->sector->floor_height;
  int rz1 = bot - renderer.state.player->viewz;
  int rz2 = bot - renderer.state.player->viewz;

  // top of wall
  int top = this->sidedef->sector->ceiling_height;
  int rz3 = top - renderer.state.player->viewz;
  int rz4 = top - renderer.state.player->viewz;

  // check if seg is behind player
  if (ry1 <= 0 && ry2 <= 0) {
    return;
  }

  // clip the wall to the player's view
  if (ry1 <= 0) {
    float d = ry1 - ry2;
    if (d == 0) {
      d = 1;
    }
    float t = ry1 / d;
    rx1 = rx1 + t * (rx2 - rx1);
    ry1 = 1;
    rz1 = rz1 + t * (rz2 - rz1);
    rz3 = rz3 + t * (rz4 - rz3);
  } else if (ry2 <= 0) {
    float d = ry2 - ry1;
    if (d == 0) {
      d = 1;
    }
    float t = ry2 / d;
    rx2 = rx2 + t * (rx1 - rx2);
    ry2 = 1;
    rz2 = rz2 + t * (rz1 - rz2);
    rz4 = rz4 + t * (rz3 - rz4);
  }

  // screen coordinates for bottom of wall
  int sx1 = (renderer.state.view_width / 2) * (1.0 - (float)rx1 / ry1);
  int sx2 = (renderer.state.view_width / 2) * (1.0 - (float)rx2 / ry2);
  int sy1 = (renderer.state.view_height / 2) * (1.0 - (float)rz1 / ry1);
  int sy2 = (renderer.state.view_height / 2) * (1.0 - (float)rz2 / ry2);

  // screen coordinates for top of wall
  int sy3 = (renderer.state.view_height / 2) * (1.0 - (float)rz3 / ry1);
  int sy4 = (renderer.state.view_height / 2) * (1.0 - (float)rz4 / ry2);

  // dont render if seg is off screen
  if ((sx1 < 0 && sx2 < 0) ||
      (sx1 >= renderer.state.view_width && sx2 >= renderer.state.view_width)) {
    return;
  }

  // backface culling and single pixel check
  if (sx1 >= sx2) {
    return;
  }

  // get the front sector
  Sector* frontsector = this->sidedef->sector;

  // get the back sector
  Sector* backsector;
  if (this->linedef->sidedef[1]) {
    backsector = (this->sidedef->sector == this->linedef->sidedef[0]->sector)
                     ? this->linedef->sidedef[1]->sector
                     : this->linedef->sidedef[0]->sector;
  }

  // check if seg is one sided
  if (!backsector) {
    // solid wall
    renderer.renderWall(sx1, sx2, sy1, sy2, sy3, sy4,
                        this->sidedef->middle_color);
    return;
  }

  // closed door
  if (backsector->ceiling_height <= frontsector->floor_height ||
      backsector->floor_height >= frontsector->ceiling_height) {
    renderer.renderWall(sx1, sx2, sy1, sy2, sy3, sy4,
                        this->sidedef->middle_color);
    return;
  }

  // // window
  // if (backsector->ceiling_height != frontsector->ceiling_height ||
  //     backsector->floor_height != frontsector->floor_height) {
  //   renderWindowSeg(renderer);
  //   return;
  // }

  // reject empty lines
  if (backsector->ceiling_color == frontsector->ceiling_color &&
      backsector->floor_color == frontsector->floor_color &&
      this->sidedef->middle_color == sf::Color::Transparent) {
    return;
  }

  // renderWindowSeg(renderer);

  renderer.renderWall(sx1, sx2, sy1, sy2, sy3, sy4,
                      this->sidedef->middle_color);
}
}  // namespace render