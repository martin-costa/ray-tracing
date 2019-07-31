#include <iostream>

#include <cstdlib>

#include "scene.hpp"
#include "framerate.hpp"
#include "inputdetector.hpp"

#define FPS 60

#define WIDTH 1600
#define HEIGHT 900

int main();

void mainLoop();

sf::Window window;

Scene scene(WIDTH, HEIGHT);