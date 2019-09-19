#include <iostream>

#include <cstdlib>

#include "scene.hpp"
#include "framerate.hpp"
#include "inputdetector.hpp"

#define FPS 60

#define WIDTH 1920
#define HEIGHT 1080

int main();

void mainLoop();

sf::Window window;

// create a scene and a view
Scene scene(WIDTH, HEIGHT);

Camera view;