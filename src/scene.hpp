#pragma once

#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>

#include <vector>

#include "vector.hpp"
#include "objects.hpp"

#define PI 3.14159265359

//// __ Camera used to define viewing position __ ////
class Camera {
private:

  // angle to verticle and horizontal
  double phi;
  double theta;

  void lookUp(double theta);
  void lookRight(double theta);

public:

  // position of camera in space
  Vector3d pos;

  // the vector that stores the upward direction (unit vec)
  Vector3d up;

  // direction camera is looking (unit vec)
  Vector3d dir;

  // direction perp. to dir and up
  Vector3d side;

  Camera();

  Camera(Vector3d pos);

  void moveCamera(double deltaTheta, double deltaPhi);

  void moveUp(double x);
  void moveRight(double x);
  void moveForward(double x);
};

//// __ Scene used to define the space __ ////
class Scene {
 private:
   int width;
   int height;

  sf::Shader rayTracingShader;
  sf::Texture sky;

public:

  // settings
  float attenuation = 5000;
  float reflectivity = 1;
  float diffuse = 0.1;

  Vector3d voidColor = Vector3d(0, 0.02, 0.04);

  // objects
  std::vector<Sphere> spheres;
  std::vector<Triangle> triangles;

  // lights
  std::vector<DirectionalLight> dirLights;
  std::vector<PointLight> pointLights;

  Scene(int width, int height);

  void drawScene(Camera cam);

  // add objects to the scene
  void addObject(Sphere sphere);
  void addObject(Triangle triangle);
  void addObject(TriangleArray triangleArray);

  void addObject(DirectionalLight dirLights);
  void addObject(PointLight pointLights);
};