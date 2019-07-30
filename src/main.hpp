#include <iostream>

#include "scene.hpp"
#include "framerate.hpp"
#include "inputdetector.hpp"

#define FPS 60

#define WIDTH 900
#define HEIGHT 600

int main();

void mainLoop();

sf::Window window;

Scene scene(WIDTH, HEIGHT);