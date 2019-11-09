#include "scene.hpp"

//// __ camera used to define viewing position __ ////

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

  side = Vector3d(1, 0, 0).rotY(theta);
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

//// __ scene used to define the space __ ////

Scene::Scene(int width, int height) {
  this->spheres = std::vector<Sphere>(0);

  this->width = width;
  this->height = height;

  rayTracingShader.loadFromFile("raytracingshader.fs", sf::Shader::Fragment);
}

void Scene::drawScene(Camera cam) {

  sf::Shader::bind(&rayTracingShader);

  // create a timer to evolve time in shader
  static int time = 0;
  time++;
  rayTracingShader.setUniform("time", time);

  // set scene size and camera
  rayTracingShader.setUniform("width", width);
  rayTracingShader.setUniform("height", height);

  rayTracingShader.setUniform("pos", cam.pos.toSf());
  rayTracingShader.setUniform("dir", cam.dir.toSf());
  rayTracingShader.setUniform("up", cam.up.toSf());
  rayTracingShader.setUniform("side", cam.side.toSf());

  // settings
  rayTracingShader.setUniform("atten", attenuation);
  rayTracingShader.setUniform("reflectivity", reflectivity);
  rayTracingShader.setUniform("diffuse", diffuse);

  rayTracingShader.setUniform("voidColor", voidColor.toSf());

  // set up spheres
  int sphereCount = spheres.size();
  rayTracingShader.setUniform("sphereCount", sphereCount);

  sf::Vector3f* spherePositions = new sf::Vector3f[sphereCount]();
  sf::Vector3f* sphereColors = new sf::Vector3f[sphereCount]();
  float* sphereRadii = new float[sphereCount]();

  for (int i = 0; i < sphereCount; i++) {
    spherePositions[i] = spheres[i].pos.toSf();
    sphereColors[i] = spheres[i].color.toSf();
    sphereRadii[i] = spheres[i].rad;
  }
  rayTracingShader.setUniformArray("spherePositions", spherePositions, sphereCount);
  rayTracingShader.setUniformArray("sphereRadii", sphereRadii, sphereCount);
  rayTracingShader.setUniformArray("sphereColors", sphereColors, sphereCount);

  // set up triangles
  int triangleCount = triangles.size();
  rayTracingShader.setUniform("triangleCount", triangleCount);

  sf::Vector3f* triP1 = new sf::Vector3f[triangleCount]();
  sf::Vector3f* triP2 = new sf::Vector3f[triangleCount]();
  sf::Vector3f* triP3 = new sf::Vector3f[triangleCount]();
  sf::Vector3f* triangleColors = new sf::Vector3f[triangleCount]();

  for (int i = 0; i < triangleCount; i++) {
    triP1[i] = triangles[i].p[0].toSf();
    triP2[i] = triangles[i].p[1].toSf();
    triP3[i] = triangles[i].p[2].toSf();
    triangleColors[i] = triangles[i].color.toSf();
  }
  rayTracingShader.setUniformArray("triP1", triP1, triangleCount);
  rayTracingShader.setUniformArray("triP2", triP2, triangleCount);
  rayTracingShader.setUniformArray("triP3", triP3, triangleCount);

  rayTracingShader.setUniformArray("triangleColors", triangleColors, triangleCount);

  // set up the lights
  int dirLightCount = this->dirLights.size();
  int pointLightCount = this->pointLights.size();

  sf::Vector3f* dirLights = new sf::Vector3f[dirLightCount]();
  sf::Vector3f* pointLights = new sf::Vector3f[pointLightCount]();

  sf::Vector3f* dirLightsColors = new sf::Vector3f[dirLightCount]();
  sf::Vector3f* pointLightsColors = new sf::Vector3f[pointLightCount]();

  for (int i = 0; i < dirLightCount; i++) {
    dirLights[i] = this->dirLights[i].dir.toSf();
    dirLightsColors[i] = this->dirLights[i].color.toSf();
  }

  for (int i = 0; i < pointLightCount; i++) {
    pointLights[i] = this->pointLights[i].pos.toSf();
    pointLightsColors[i] = this->pointLights[i].color.toSf();
  }

  rayTracingShader.setUniform("dirLightCount", dirLightCount);
  rayTracingShader.setUniform("pointLightCount", pointLightCount);

  rayTracingShader.setUniformArray("dirLights", dirLights, dirLightCount);
  rayTracingShader.setUniformArray("pointLights", pointLights, pointLightCount);

  rayTracingShader.setUniformArray("dirLightsColors", dirLightsColors, dirLightCount);
  rayTracingShader.setUniformArray("pointLightsColors", pointLightsColors, pointLightCount);

  // free the memory
  delete[] spherePositions;
  delete[] sphereRadii;
  delete[] sphereColors;

  delete[] triP1;
  delete[] triP2;
  delete[] triP3;
  delete[] triangleColors;

  delete[] dirLights;
  delete[] pointLights;
  delete[] dirLightsColors;
  delete[] pointLightsColors;

  glBegin(GL_QUADS);

  glVertex2i(-width / 2, -height/2);
  glVertex2i(width / 2, -height / 2);
  glVertex2i(width / 2, height / 2);
  glVertex2i(-width / 2, height / 2);

  glEnd();
}

// add objects to the scene
void Scene::addObject(Sphere sphere) { spheres.push_back(sphere); }

void Scene::addObject(Triangle triangle) { triangles.push_back(triangle); }

void Scene::addObject(TriangleArray triangleArray) { 

  std::vector<Triangle> tris = triangleArray.getTriangles();

  for (int i = 0; i < tris.size(); i++) {
    addObject(tris[i]);
  }
}

void Scene::addObject(DirectionalLight dirLight) { dirLights.push_back(dirLight); }

void Scene::addObject(PointLight pointLight) { pointLights.push_back(pointLight); }