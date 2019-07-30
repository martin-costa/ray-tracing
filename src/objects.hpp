#pragma once

#include "linearalgebra.hpp"

//class to represent a line in 3d
class Line {
public:

  //the starting point of the line
  Vector3d orig;

  //the direction the line points in (ideally ||dir|| = 1)
  Vector3d dir;

  Line(Vector3d orig, Vector3d dir);
};

//the objects that can be put into the scene
class Object {
public:

  virtual double closestIntersection(Line line) = 0;
};

class Sphere : Object {
public:

  Vector3d pos;

  Vector3d color;

  double rad;

  Sphere(Vector3d pos, double rad, Vector3d color);

  double closestIntersection(Line line);
};