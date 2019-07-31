#pragma once

#include "vector.hpp"

//the objects that can be put into the scene

class Sphere {
public:

  Vector3d pos;

  Vector3d color;

  double rad;

  Sphere();

  Sphere(Vector3d pos, double rad, Vector3d color);
};