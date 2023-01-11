#include "game/DoomGame.hpp"

#include <iostream>

// Entry point into the program
int main(int argc, char* argv[]) {
  // Create the game
  game::DoomGame game;

  // Load the map from command line argument if one is provided
  if (argc > 1) {
    game.loadMap(argv[1]);
  }

  // Run the game
  game.run();

  return 0;
}
