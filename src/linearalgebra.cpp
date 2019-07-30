#include "linearalgebra.hpp"

//constructors
Vector3d::Vector3d(double x, double y, double z) {
  this->x = x;
  this->y = y;
  this->z = z;
}

Vector3d::Vector3d() {
  this->x = 0;
  this->y = 0;
  this->z = 0;
}

//standard vector field ops.
Vector3d Vector3d::operator+(Vector3d v) {
  return Vector3d(x + v.x, y + v.y, z + v.z);
}

void Vector3d::operator+=(Vector3d v) {
  x += v.x;
  y += v.y;
  z += v.z;
}

Vector3d Vector3d::operator-(Vector3d v) {
  return Vector3d(x - v.x, y - v.y, z - v.z);
}

Vector3d Vector3d::operator-() {
  return Vector3d(-x, -y, -z);
}

void Vector3d::operator-=(Vector3d v) {
  x -= v.x;
  y -= v.y;
  z -= v.z;
}

Vector3d Vector3d::operator*(double a) {
  return Vector3d(x * a, y * a, z * a);
}

void Vector3d::operator*=(double a) {
  x *= a;
  y *= a;
  z *= a;
}

Vector3d Vector3d::operator/(double a) {
  return Vector3d(x / a, y / a, z / a);
}

void Vector3d::operator/=(double a) {
  x /= a;
  y /= a;
  z /= a;
}

//R^3 vector ops
Vector3d Vector3d::normalize() {
  return (*this) / magnitude();
}

double Vector3d::magnitude() {
  return sqrt(x * x + y * y + z * z);
}

double Vector3d::dot(Vector3d v) {
  return x * v.x + y * v.y + z * v.z;
}

Vector3d Vector3d::cross(Vector3d v) {
  return Vector3d(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}