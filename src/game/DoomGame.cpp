#include "game/DoomGame.hpp"

#include "game/Player.hpp"
#include "render/DoomRenderer.hpp"
#include "wad/WadFile.hpp"

namespace game {
DoomGame::DoomGame(int res)
    : window(sf::VideoMode(1280, 960), "Doom"), renderer(res) {
  // Check if the resolution is valid
  if (res < 1 || res > 4) {
    throw std::invalid_argument("Resolution must be between 1 and 4.");
  }
  // Set the window's framerate limit
  window.setFramerateLimit(24);
}

void DoomGame::run() {
  // Setup

  // Start the main loop
  while (window.isOpen()) {
    // Handle events
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }
    // Handle keyboard input
    handleKeyboardInput();
    // Display
    display();
  }
}

void DoomGame::loadMap(const std::string& wad_path) {
  // Load the wad file
  wad::WadFile wad(wad_path);

  // Load the map
  this->renderer.loadMap(wad);
}

void DoomGame::display() {
  // Clear the window
  window.clear();

  // Call the renderer
  renderer.renderWindow(this->window);

  // Display the window
  window.display();
}

void DoomGame::handleKeyboardInput() { renderer.state.player->movePlayer(); }
}  // namespace game