#ifndef DOOMGAME_HPP
#define DOOMGAME_HPP

#include "render/DoomRenderer.hpp"

namespace game {
class DoomGame {
  // Game Window
  sf::RenderWindow window;
  // Game Renderer
  render::DoomRenderer renderer;

 public:
  // Constructor
  // res: resolution scale (1 = 320x240, 2 = 640x480, 3 = 960x720, 4 = 1280x960)
  // can only be 1, 2, 3, or 4
  DoomGame(int res = 1);

  void run();
  void loadMap(const std::string& wad_path);
  void display();
  void handleKeyboardInput();
};
}  // namespace game

#endif