#pragma once

#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

#include <vector>

#include "linearalgebra.hpp"
#include "objects.hpp"

//camera class to define a viewing position
class Camera {
public:

  //position of camera in space
  Vector3d pos;

  //direction camera is looking (ideally ||dir|| = 1)
  Vector3d dir;

  Camera(Vector3d pos);

  Camera(Vector3d pos, Vector3d dir);
};

//scene class to define a space where objects can exist
class Scene {

  //the main camera of the scene
  Camera view;

  //objects
  std::vector<Sphere> objects;

  //get c
};