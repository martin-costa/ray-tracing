#include "scene.hpp"

Camera::Camera(Vector3d pos) {
  this->pos = pos;
}

Camera::Camera(Vector3d pos, Vector3d dir) {
  this->pos = pos;
  this->dir = dir;
}