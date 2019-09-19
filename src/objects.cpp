#include "objects.hpp"

PointLight::PointLight(Vector3d pos, Vector3d color) {
  this->pos = pos;
  this->color = color;
}

PointLight::PointLight() {}

Sphere::Sphere(Vector3d pos, double rad, Vector3d color) {
  this->pos = pos;
  this->rad = rad;
  this->color = color;
}

Sphere::Sphere() {}

Triangle::Triangle(Vector3d p1, Vector3d p2, Vector3d p3, Vector3d color) {
  this->p1 = p1;
  this->p2 = p2;
  this->p3 = p3;
  this->color = color;
}

Triangle::Triangle() {}
