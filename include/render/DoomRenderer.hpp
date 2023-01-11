#ifndef DOOMRENDERER_HPP
#define DOOMRENDERER_HPP

#include <SFML/Graphics.hpp>

#include "render/RenderState.hpp"

namespace wad {
class WadFile;
}

namespace render {

class DoomRenderer {
  // TODO: make private
 public:
  // Pixel buffer
  sf::Uint8* pixels;
  // Window dimensions
  int window_width;
  int window_height;
  // Render state
  RenderState state;

 public:
  // Constructor
  DoomRenderer(int res = 1);
  // Destructor
  ~DoomRenderer();

  void init();

  void loadMap(wad::WadFile& wad);

  // Renders the pixel buffer to the window
  void renderWindow(sf::RenderWindow& window);

  // Methods that draw to the pixel buffer

  // Draws a pixel
  void renderPixel(int x, int y, sf::Uint8 r, sf::Uint8 g, sf::Uint8 b);
  void renderPixel(int x, int y, sf::Color color);

  // Draws a wall using the given coordinates
  // x1, x2: x coordinates of the wall
  // b1, b2: y coordinates of the bottom of the wall
  // t1, t2: y coordinates of the top of the wall
  // surface: 0 = none, 1 = floor, 2 = ceiling, 3 = both
  void renderWall(int x1, int x2, int b1, int b2, int t1, int t2,
                  sf::Color color, int surface = 0);

  // Draws the player's view
  void renderPlayerView();
};

}  // namespace render

#endif