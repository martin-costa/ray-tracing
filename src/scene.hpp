#pragma once

#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>

#include <vector>

#include "vector.hpp"
#include "objects.hpp"

#define PI 3.14159265359

//camera class to define a viewing position
class Camera {
private:

  //angle to verticle and horizontal
  double phi;
  double theta;

  void lookUp(double theta);
  void lookRight(double theta);

public:

  //position of camera in space
  Vector3d pos;

  //the vector that stores the upward direction (unit vec)
  Vector3d up;

  //direction camera is looking (unit vec)
  Vector3d dir;

  //direction perp. to dir and up
  Vector3d side;

  Camera();

  Camera(Vector3d pos);

  void moveCamera(double deltaTheta, double deltaPhi);

  void moveUp(double x);
  void moveRight(double x);
  void moveForward(double x);
};

//scene class to define a space where objects can exist
class Scene {
 private:
   int width;
   int height;

  //objects
  std::vector<Sphere> objects;

  sf::Shader rayTracingShader;

public:

  //the main camera of the scene
  Camera view;

  Scene(int width, int height);

  void drawScene();

  void addObject(Sphere sphere);
};