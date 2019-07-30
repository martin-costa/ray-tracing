#include <iostream>

#include "scene.hpp"
#include "framerate.hpp"

#define FPS 60

#define WIDTH 900
#define HEIGHT 600

int main();

void mainLoop();

sf::Window window;

double* pixles = new double[WIDTH * HEIGHT]();
Sphere sphere(Vector3d(0, 0, 1300), 255, Vector3d(1, 0, 0));
Sphere sphere2(Vector3d(0, -500, 1300), 50, Vector3d(0, 1, 0));