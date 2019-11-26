#include "objects.hpp"

#include <iostream>

typedef Vector3d vec;

PointLight::PointLight(vec pos, vec color) {
  this->pos = pos;
  this->color = color;
}

PointLight::PointLight(vec pos) : PointLight(pos, vec(1,1,1)) {}

PointLight::PointLight() {}

DirectionalLight::DirectionalLight(vec dir, vec color) {
  this->dir = dir;
  this->color = color;
}

DirectionalLight::DirectionalLight(vec dir) : DirectionalLight(dir, vec(1, 1, 1)) {}

DirectionalLight::DirectionalLight() {}

Sphere::Sphere(vec pos, double rad, vec color) {
  this->pos = pos;
  this->rad = rad;
  this->color = color;
}

Sphere::Sphere() {}

Triangle::Triangle(vec p1, vec p2, vec p3, vec color) {
  this->p[0] = p1;
  this->p[1] = p2;
  this->p[2] = p3;
  this->color = color;
}

Triangle::Triangle() {}

TriangleArray::TriangleArray(vec* vertices, int vertexCount, int* indices, int indexCount, vec color) {
  this->vertexCount = vertexCount;
  this->indexCount = indexCount;
  this->color = color;

  this->vertices = new vec[vertexCount];
  this->indices = new int[indexCount];

  for (int i = 0; i < vertexCount; i++)
    this->vertices[i] = vertices[i];

  for (int i = 0; i < indexCount; i++)
    this->indices[i] = indices[i];
}

TriangleArray::TriangleArray() {}

TriangleArray::~TriangleArray() {
  //delete[] this->vertices;
  //delete[] this->indices;
}

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

TriangleArray cuboid(vec pos, vec dx, vec dy, vec dz, vec color) {

  vec verts[] = { pos, pos + dz, pos + dx + dz, pos + dx,
                  pos + dy, pos + dy + dz, pos + dx + dy + dz, pos + dx + dy };

  int indices[] = { 0,1,3,  2,1,3,  4,5,6,  4,7,6,  0,3,7,  0,4,7,  1,2,6,  1,5,6,  3,2,6,  3,6,7,  0,1,5,  0,4,5 };

  return TriangleArray(verts, 8, indices, 36, color);
}
