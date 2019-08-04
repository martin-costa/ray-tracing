#pragma once

#include "vector.hpp"

//the objects that can be put into the scene

//a light that casts light in all directions
class PointLight {
public:

  Vector3d pos;

  PointLight(Vector3d pos);

  PointLight();
};

//a sphere
class Sphere {
public:

  Vector3d pos;

  Vector3d color;

  double rad;

  Sphere(Vector3d pos, double rad, Vector3d color);

  Sphere();
};