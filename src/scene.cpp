#include "scene.hpp"

//// __ Camera used to define viewing position __ ////

Camera::Camera() : Camera(Vector3d()) {}

Camera::Camera(Vector3d pos) {
  this->pos = pos;

  this->dir = Vector3d(0, 0, 1);
  this->up = Vector3d(0, 1, 0);

  this->phi = 0;
  this->theta = 0;

  this->side = Vector3d(1, 0, 0);
}

void Camera::moveCamera(double deltaTheta, double deltaPhi) {
  lookUp(deltaPhi);
  lookRight(deltaTheta);

  side = Vector3d(1 ,0 , 0).rotY(theta);
  up = Vector3d(0, 1, 0).rotY(theta).rotateAbout(side, phi);
  dir = Vector3d(0, 0, 1).rotY(theta).rotateAbout(side, phi);
}

void Camera::lookUp(double delta) {
  phi += delta;
  if (phi >= PI / 2) phi = PI / 2 - 0.00001;
  if (phi <= -PI / 2) phi = -PI / 2 + 0.00001;
}

void Camera::lookRight(double delta) {
  theta += delta;
  if (theta > 2 * PI) theta -= 2 * PI;
  if (theta < 0) theta += 2 * PI;
}

void Camera::moveUp(double x) {
  pos.y += x;
}

void Camera::moveRight(double x) {
  pos += side * x;
}

void Camera::moveForward(double x) {
  Vector3d dirxz(dir.x, 0, dir.z);
  pos += dirxz.normalize() * x;
}

//// __ Scene used to define the space __ ////

Scene::Scene(int width, int height) {
  this->objects = std::vector<Sphere>(0);

  this->width = width;
  this->height = height;

  rayTracingShader.loadFromFile("raytracingshader.fs", sf::Shader::Fragment);
}

void Scene::drawScene() {

  sf::Shader::bind(&rayTracingShader);

  int sphereCount = objects.size();

  rayTracingShader.setUniform("width", width);
  rayTracingShader.setUniform("height", height);

  rayTracingShader.setUniform("sphereCount", sphereCount);

  rayTracingShader.setUniform("light", light);

  rayTracingShader.setUniform("pos", view.pos.toSf());
  rayTracingShader.setUniform("dir", view.dir.toSf());
  rayTracingShader.setUniform("up", view.up.toSf());
  rayTracingShader.setUniform("side", view.side.toSf());

  sf::Vector3f* positions = new sf::Vector3f[sphereCount]();
  float* radii = new float[sphereCount]();
  sf::Vector3f* colors = new sf::Vector3f[sphereCount]();

  for (int i = 0; i < sphereCount; i++) {
    positions[i] = objects[i].pos.toSf();
    radii[i] = objects[i].rad;
    colors[i] = objects[i].color.toSf();
  }
  rayTracingShader.setUniformArray("spherePositions", positions, sphereCount);

  rayTracingShader.setUniformArray("sphereRadii", radii, sphereCount);

  rayTracingShader.setUniformArray("sphereColors", colors, sphereCount);

  glBegin(GL_QUADS);

  glVertex2i(-width / 2, -height/2);
  glVertex2i(width / 2, -height / 2);
  glVertex2i(width / 2, height / 2);
  glVertex2i(-width / 2, height / 2);

  glEnd();
}

void Scene::addObject(Sphere sphere) {
  objects.push_back(sphere);
}