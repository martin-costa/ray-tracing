#include "objects.hpp"

#include <iostream>

PointLight::PointLight(Vector3d pos, Vector3d color) {
  this->pos = pos;
  this->color = color;
}

PointLight::PointLight(Vector3d pos) : PointLight(pos, Vector3d(1,1,1)) {}

PointLight::PointLight() {}

DirectionalLight::DirectionalLight(Vector3d dir, Vector3d color) {
  this->dir = dir;
  this->color = color;
}

DirectionalLight::DirectionalLight(Vector3d dir) : DirectionalLight(dir, Vector3d(1, 1, 1)) {}

DirectionalLight::DirectionalLight() {}

Sphere::Sphere(Vector3d pos, double rad, Vector3d color) {
  this->pos = pos;
  this->rad = rad;
  this->color = color;
}

Sphere::Sphere() {}

Triangle::Triangle(Vector3d p1, Vector3d p2, Vector3d p3, Vector3d color) {
  this->p[0] = p1;
  this->p[1] = p2;
  this->p[2] = p3;
  this->color = color;
}

Triangle::Triangle() {}

TriangleArray::TriangleArray(Vector3d* vertices, int vertexCount, int* indices, int indexCount, Vector3d color) {
  this->vertexCount = vertexCount;
  this->indexCount = indexCount;

  this->vertices = vertices;
  this->indices = indices;

  this->color = color;
}

TriangleArray::TriangleArray() {}

std::vector<Triangle> TriangleArray::getTriangles() {
  int triangleIndex = 0;
  int vertexIndex = 0;

  std::vector<Triangle> triangles = std::vector<Triangle>(indexCount / 3);

  while (triangleIndex < indexCount / 3) {
    triangles[triangleIndex++] = Triangle(vertices[indices[vertexIndex]], vertices[indices[vertexIndex + 1]], vertices[indices[vertexIndex + 2]], color);
    vertexIndex += 3;
  }

  return triangles;
}
