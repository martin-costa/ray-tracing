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

  // objects
  this->spheres = std::vector<Sphere>(0);
  this->triangles = std::vector<Triangle>(0);

  this->objects = std::vector<std::tuple<Vector3d, int, int, int, int>>(0);

  // lights
  this->dirLights = std::vector<DirectionalLight>(0);
  this->pointLights = std::vector<PointLight>(0);

  this->width = width;
  this->height = height;

  rayTracingShader.loadFromFile("raytracingshader.fs", sf::Shader::Fragment);

  //sky.loadFromFile("skydome.png");
  //grass.loadFromFile("grassmc.png");

  //grassMean = getTextureMean(grass);
}

void Scene::drawScene(Camera cam) {

  // create a timer to evolve time in shader
  static int time = 0;
  time++;
  //rayTracingShader.setUniform("time", time);

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

  // pass texture
  //rayTracingShader.setUniform("useTextures", false);
  //rayTracingShader.setUniform("sky", sky);
  //rayTracingShader.setUniform("grass", grass);

  //rayTracingShader.setUniform("grassMean", grassMean.toSf());

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

  // bind the shader
  sf::Shader::bind(&rayTracingShader);

  glBegin(GL_QUADS);

 // glTexCoord2f(0.f, 1.f);
  glVertex2i(-width / 2, -height/2);

  //glTexCoord2f(1.f, 1.f);
  glVertex2i(width / 2, -height / 2);

  //glTexCoord2f(1.f, 0.f);
  glVertex2i(width / 2, height / 2);

  //glTexCoord2f(0.f, 0.f);
  glVertex2i(-width / 2, height / 2);

  glEnd();
}

// interact with scene
void Scene::rotateObj(int i, float dx, float dy, float dz) {

  std::tuple<Vector3d, int, int, int, int> tup = objects[i];

  Vector3d pos = std::get<0>(tup);

  for (int i = 0; i < std::get<2>(tup); i++) {
    spheres[std::get<1>(tup) + i].pos = (pos - spheres[std::get<1>(tup) + i].pos).rotX(dx) + pos;
    spheres[std::get<1>(tup) + i].pos = (pos - spheres[std::get<1>(tup) + i].pos).rotY(dy) + pos;
    spheres[std::get<1>(tup) + i].pos = (pos - spheres[std::get<1>(tup) + i].pos).rotZ(dz) + pos;
  }

  for (int i = 0; i < std::get<4>(tup); i++) {
    for (int j = 0; j < 3; j++) {
      triangles[std::get<3>(tup) + i].p[j] = (pos - triangles[std::get<3>(tup) + i].p[j]).rotX(dx) + pos;
      triangles[std::get<3>(tup) + i].p[j] = (pos - triangles[std::get<3>(tup) + i].p[j]).rotY(dy) + pos;
      triangles[std::get<3>(tup) + i].p[j] = (pos - triangles[std::get<3>(tup) + i].p[j]).rotZ(dz) + pos;
    }
  }
}

// add objects to the scene
void Scene::addObject(Sphere sphere) { 
  addToList(sphere.pos, spheres.size(), 1, 0, 0);
  spheres.push_back(sphere);
}

void Scene::addObject(Triangle triangle) { 
  addToList(triangle.p[0], 0, 0, triangles.size(), 1);
  triangles.push_back(triangle);
}

void Scene::addObject(TriangleArray triangleArray) { 

  std::vector<Triangle> tris = triangleArray.getTriangles();
  addToList(triangleArray.pos, 0, 0, triangles.size(), tris.size());

  for (int i = 0; i < tris.size(); i++) {
    triangles.push_back(tris[i]);
  }
}

void Scene::addObject(DirectionalLight dirLight) { 
  dirLights.push_back(dirLight);
}

void Scene::addObject(PointLight pointLight) { 
  pointLights.push_back(pointLight);
}

void Scene::addToList(Vector3d pos, int s_start, int s_count, int t_start, int t_count) {
  objects.push_back(std::tuple<Vector3d, int, int, int, int>(pos, s_start, s_count, t_start, t_count));
}

Vector3d getTextureMean(sf::Texture tex) {

  sf::Image image = tex.copyToImage();

  sf::Color temp = sf::Color(0, 0, 0);
  float r = 0.0, g = 0.0, b = 0.0;
  
  for (int i = 0; i < image.getSize().x; i++) {
    for (int j = 0; j < image.getSize().y; j++) {
      temp = image.getPixel(i, j);

      r += temp.r;
      g += temp.g;
      b += temp.b;
    }
  }

  return Vector3d(r,g,b) / (float)(image.getSize().x * image.getSize().y * 255);
}