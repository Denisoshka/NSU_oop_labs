#include "process.hpp"

int main(int argc, char **argv) {
  std::string settingsPath = "basicmap.txt";
  if( argc > 1 ) {
    settingsPath = argv[1];
  }
  gameProcess::gameProcess game{std::move(settingsPath)};
  game.process();

  return 0;
}