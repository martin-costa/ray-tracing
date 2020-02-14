#pragma once

#include "vector.hpp"

//// __ the objects that can be put into the scene __ ////

//// __ light (creating) objects __ ////

// a point that casts light rays in all directions
class PointLight {
public:

  Vector3d pos;

  Vector3d color;

  int brightness; // in [0, 1]

  PointLight(Vector3d pos, Vector3d color);

  PointLight(Vector3d pos);

  PointLight();
};

// a direction where light rays are cast in parallel
class DirectionalLight {
public:

  Vector3d dir;

  Vector3d color;

  int brightness; // in [0, 1]

  DirectionalLight(Vector3d dir, Vector3d color);

  DirectionalLight(Vector3d dir);

  DirectionalLight();
};

//// __ opaque reflective objects __ ////

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

  Vector3d p[3];

  Vector3d color;

  Triangle(Vector3d p1, Vector3d p2, Vector3d p3, Vector3d color);

  Triangle();
};

// a triangle array
class TriangleArray {
public:

  Vector3d* vertices;

  int* indices;

  int vertexCount;

  int indexCount;

  Vector3d pos;

  Vector3d color;

  TriangleArray(Vector3d* vertices, int vertexCount, int* indices, int indexCount, Vector3d pos, Vector3d color);

  TriangleArray();

  ~TriangleArray();

  std::vector<Triangle> getTriangles();
};

TriangleArray cuboid(Vector3d pos, Vector3d dx, Vector3d dy, Vector3d dz, Vector3d color);