#include "game.h"
#include <iostream>

int main(int argc, char *argv[])
{
  Game game;
  std::cout << "Start game";
  bool success = game.initialize();
  if (success) {
    game.runLoop();
  }
  game.shutDown();
  return 0;
}
