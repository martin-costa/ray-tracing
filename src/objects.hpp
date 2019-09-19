#pragma once

#include "vector.hpp"

//// __ the objects that can be put into the scene __ ////

// a light that casts rays in all directions
class PointLight {
public:

  Vector3d pos;

  Vector3d color;

  PointLight(Vector3d pos, Vector3d color);

  PointLight();
};

// a sphere
class Sphere {
public:

  Vector3d pos;

  Vector3d color;

  double rad;

  Sphere(Vector3d pos, double rad, Vector3d color);

  Sphere();
};

// a triangle
class Triangle {
public:

  Vector3d p1, p2, p3;

  Vector3d color;

  Triangle(Vector3d p1, Vector3d p2, Vector3d p3, Vector3d color);

  Triangle();
};