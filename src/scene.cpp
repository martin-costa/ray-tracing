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
}

void Scene::drawScene() {

  float ratio = (float)width / (float)height;

  glBegin(GL_POINTS);

  for (int i = -width / 2; i < width / 2; i++) {
    for (int j = -height / 2; j < height / 2; j++) {

      int index = -1;
      double d = -1;

      for (int k = 0; k < objects.size(); k++) {
        double d2 = objects[k].closestIntersection(Line(view.pos, (view.dir + view.side * (float)i * ratio / (width / 2) + view.up * (float)j / (height / 2))));
        if (d2 > 0 && (d2 < d || d < 0)) {
          d = d2;
          index = k;
        }
      }

      if (d > 0) {
        Vector3d v = view.pos + (view.dir + view.side * (float)i * ratio / (width / 2) + view.up * (float)j / (height / 2)).normalize() * d;
        double f = (v - objects[index].pos).normalize().dot(Vector3d(0, 1, 0));

        Vector3d c = objects[index].color;
        glColor3f(c.x * f, c.y * f, c.z * f);
        glVertex2i(i, j);
      }
    }
  }

  glEnd();
}

void Scene::addObject(Sphere sphere) {
  objects.push_back(sphere);
}