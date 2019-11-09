#include <iostream>

#include <cstdlib>

#include "scene.hpp"
#include "framerate.hpp"
#include "inputdetector.hpp"

#define FPS 60

#define WIDTH 1980
#define HEIGHT 1080

int main();

void launch();

void mainLoop();

sf::Window window;

// create a scene and a view
Scene scene(WIDTH, HEIGHT);

Camera view(Vector3d(-200, 400, 200));