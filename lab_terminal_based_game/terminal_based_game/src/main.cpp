#include "process.hpp"

int main(int argc, char **argv) {
//  std::string settingsPath = "game_screen.txt";
//  if( argc > 1 ) {
//    settingsPath = argv[1];
//  }
  gameProcess::gameProcess game{};
  game.process();

  return 0;
}