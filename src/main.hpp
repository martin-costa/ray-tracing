#include <iostream>

#include <cstdlib>

#include "scene.hpp"
#include "framerate.hpp"
#include "inputdetector.hpp"

#define FPS 60

#define WIDTH 1980
#define HEIGHT 1080

#define SCENE_IDX 1

int main();

void mainLoop(sf::Event evnt);

sf::Window window;

// create a scene and a view
Scene scene(WIDTH, HEIGHT);

Camera view;

// create object scenes
void createScene(int i);
void updateScene(int i);

void createScene1();
void updateScene1();

void createScene2();
void updateScene2();