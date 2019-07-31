#include "vector.hpp"

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

//R^3 vector ops.
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

Vector3d Vector3d::rotX(double theta) {
  return Vector3d(x, y * cos(theta) - z * sin(theta), y * sin(theta) + z * cos(theta));
}

Vector3d Vector3d::rotY(double theta) {
  return Vector3d(x * cos(theta) + z * sin(theta), y, -x * sin(theta) + z * cos(theta));
}

Vector3d Vector3d::rotZ(double theta) {
  return Vector3d(x * cos(theta) - y * sin(theta), x * sin(theta) + y * cos(theta), z);
}

Vector3d Vector3d::rotateAbout(Vector3d v, double theta) {
  double c = cos(theta);
  double s = sin(theta);

  double ux = (c + v.x * v.x * (1 - c)) * x + (v.x * v.y * (1 - c) - v.z * s) * y + (v.x * v.z * (1 - c) + v.y * s) * z;
  double uy = (v.y * v.x * (1 - c) + v.z * s) * x + (c + v.y * v.y * (1 - c)) * y + (v.y * v.z * (1 - c) - v.x * s) * z;
  double uz = (v.z * v.x * (1 - c) - v.y * s) * x + (v.z * v.y * (1 - c) + v.x * s) * y + (c + v.z * v.z * (1 - c)) * z;

  return Vector3d(ux, uy, uz);
}

//sfml ops.
sf::Vector3f Vector3d::toSf() {
  return sf::Vector3f(x, y, z);
}

