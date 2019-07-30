#include "objects.hpp"

Line::Line(Vector3d orig, Vector3d dir) {
  this->orig = orig;
  this->dir = dir;
}

Sphere::Sphere(Vector3d pos, double rad, Vector3d color) {
  this->pos = pos;
  this->rad = rad;
  this->color = color;
}

double Sphere::closestIntersection(Line line) {
  Vector3d CO = line.orig - pos;
  double x = -(line.dir.dot(CO));
  double y = pow(line.dir.dot(CO), 2) - pow(CO.magnitude(), 2) + rad * rad;
  if (y < 0) return -1;

  y = sqrt(y);
  double d1 = x - y;
  if (d1 >= 0) return d1;
  else return x + y;
}