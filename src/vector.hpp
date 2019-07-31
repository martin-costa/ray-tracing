#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

//3d real vector
class Vector3d {
public:
  double x;
  double y;
  double z;

  //constructors
  Vector3d(double x, double y, double z);

  Vector3d();

  //standard vector field ops.
  Vector3d operator+(Vector3d v);

  void operator+=(Vector3d v);

  Vector3d operator-(Vector3d v);

  Vector3d operator-();

  void operator-=(Vector3d v);

  Vector3d operator*(double a);

  void operator*=(double a);

  Vector3d operator/(double a);

  void operator/=(double a);

  //R^3 vector ops.
  Vector3d normalize();

  double magnitude();

  double dot(Vector3d v);

  Vector3d cross(Vector3d v);

  Vector3d rotX(double theta);
  Vector3d rotY(double theta);
  Vector3d rotZ(double theta);

  Vector3d rotateAbout(Vector3d v, double theta);

  //sfml ops.
  sf::Vector3f toSf();
};