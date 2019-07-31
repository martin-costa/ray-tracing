#include "objects.hpp"

Sphere::Sphere(Vector3d pos, double rad, Vector3d color) {
  this->pos = pos;
  this->rad = rad;
  this->color = color;
}

Sphere::Sphere() {}