#include "render/DoomRenderer.hpp"

#include <cmath>

#include "game/Player.hpp"
#include "misc/Utils.hpp"
#include "render/Render.hpp"
#include "wad/WadFile.hpp"

namespace render {
DoomRenderer::DoomRenderer(int res) {
  int width = 320 * res;
  int height = 240 * res;
  float pixel_scale = 4.0f / res;

  // Set the render state
  this->state = RenderState();

  // Set the screen dimensions
  this->state.view_width = width;
  this->state.view_height = height;
  this->state.pixel_scale = pixel_scale;

  // Set window dimensions
  this->window_width = width * this->state.pixel_scale;
  this->window_height = height * this->state.pixel_scale;

  // Create the pixel array
  this->pixels = new sf::Uint8[window_width * window_height * 4];

  // // Initialize the renderer
  // this->init();
}

DoomRenderer::~DoomRenderer() { delete[] this->pixels; }

void DoomRenderer::init() {
  // Create the player
  this->state.player = new game::Player(20, 50, 20, 90);

  // Create segs
  Seg* seg1 = new Seg();
  seg1->v1 = new Vertex(40, 40);
  seg1->v2 = new Vertex(40, 70);
  seg1->sidedef = new SideDef(sf::Color::Yellow, sf::Color::Yellow,
                              sf::Color::Yellow, nullptr);
  this->state.segs.push_back(seg1);

  Seg* seg2 = new Seg();
  seg2->v1 = new Vertex(40, 70);
  seg2->v2 = new Vertex(70, 70);
  seg2->sidedef = new SideDef(sf::Color::Green, sf::Color::Green,
                              sf::Color::Green, nullptr);
  this->state.segs.push_back(seg2);
}

void DoomRenderer::loadMap(wad::WadFile& wad) {
  wad.loadRenderState(this->state);
}

void DoomRenderer::renderPixel(int x, int y, sf::Uint8 r, sf::Uint8 g,
                               sf::Uint8 b) {
  // Check if the pixel is out of bounds
  if (x < 0 || x >= this->state.view_width || y < 0 ||
      y >= this->state.view_height) {
    return;
    throw std::invalid_argument("Pixel with coordinates (" + std::to_string(x) +
                                ", " + std::to_string(y) +
                                ") is out of bounds.");
  }

  // Scale the pixel
  x *= this->state.pixel_scale;
  y *= this->state.pixel_scale;

  // Check if the pixel has been drawn
  if (this->pixels[(x + y * this->window_width) * 4] != 0) {
    return;
  }

  // Set the pixel
  for (int i = 0; i < this->state.pixel_scale; i++) {
    for (int j = 0; j < this->state.pixel_scale; j++) {
      this->pixels[(x + i + (y + j) * this->window_width) * 4] = r;
      this->pixels[(x + i + (y + j) * this->window_width) * 4 + 1] = g;
      this->pixels[(x + i + (y + j) * this->window_width) * 4 + 2] = b;
      this->pixels[(x + i + (y + j) * this->window_width) * 4 + 3] = 255;
    }
  }
}

void DoomRenderer::renderPixel(int x, int y, sf::Color color) {
  this->renderPixel(x, y, color.r, color.g, color.b);
}

void DoomRenderer::renderWall(int x1, int x2, int b1, int b2, int t1, int t2,
                              sf::Color color, int surface) {
  // x change
  int dx = x2 - x1;
  dx = dx == 0 ? 1 : dx;
  // bottom y change
  int db = b2 - b1;
  // top y change
  int dt = t2 - t1;

  // determine the x direction of the wall
  int x_dir = x1 < x2 ? 1 : -1;

  // save the original x1
  int x1_orig = x1;

  // cap the x coordinates to the screen
  x1 = std::min(this->state.view_width - 1, std::max(x1, 0));
  x2 = std::min(this->state.view_width - 1, std::max(x2, 0));

  for (int x = x1; x != x2 + x_dir; x += x_dir) {
    // calculate y coordinates for bottom and top of wall
    int yb = b1 + db * ((float)(x - x1_orig) / dx);
    int yt = t1 + dt * ((float)(x - x1_orig) / dx);

    // determine the y direction of the wall
    int y_dir = yb < yt ? 1 : -1;

    // cap the y coordinates to the screen
    yb = std::min(this->state.view_height - 1, std::max(yb, 0));
    yt = std::min(this->state.view_height - 1, std::max(yt, 0));

    // render a column
    for (int y = yb; y != yt + y_dir; y += y_dir) {
      this->renderPixel(x, y, color);
    }
  }
}

void DoomRenderer::renderPlayerView() {
  // Draw a pixel for each seg
  for (Seg* seg : this->state.segs) {
    this->renderPixel(seg->v1->x, seg->v1->y, sf::Color::White);
    this->renderPixel(seg->v2->x, seg->v2->y, sf::Color::White);
  }

  // Draw a pixel at the player's position
  this->renderPixel(this->state.player->x, this->state.player->y,
                    sf::Color::White);

  // Draw angle
  const float rad = misc::degToRad(this->state.player->angle);
  int dx = std::sin(rad) * 5.0;
  int dy = std::cos(rad) * 5.0;
  this->renderPixel(this->state.player->x + dx, this->state.player->y + dy,
                    sf::Color::Red);

  // Actual drawing from here on out

  // Draw the head node
  this->state.nodes[0]->renderNode(*this);

  // Draw the planes (floor and ceiling)

  // Draw the things
}

void DoomRenderer::renderWindow(sf::RenderWindow& window) {
  // Render the player view
  renderPlayerView();

  // Create a texture
  sf::Texture texture;
  texture.create(this->window_width, this->window_height);

  // Update the texture
  texture.update(this->pixels);

  // Create a sprite
  sf::Sprite sprite;
  sprite.setTexture(texture);

  // Draw the sprite
  window.draw(sprite);

  // Reset the pixel buffer
  for (int i = 0; i < this->window_width * this->window_height * 4; i++) {
    this->pixels[i] = 0;
  }
}
}  // namespace render