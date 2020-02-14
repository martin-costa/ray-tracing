#include "main.hpp"

typedef Vector3d vec;

int main() {

  sf::Event evnt;

  // create the window for opengl
  window.create(sf::VideoMode(WIDTH, HEIGHT), "Ray Tracer", sf::Style::Titlebar);
  window.setMouseCursorVisible(false);
  window.setMouseCursorGrabbed(true);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // set normal coord system
  glScalef(1.0f, -1.0f, 1.0f);
  glOrtho(0, WIDTH, HEIGHT, 0, 0, 1);
  glTranslatef(WIDTH / 2, HEIGHT / 2, 0.0f);
  glMatrixMode(GL_MODELVIEW);

  glEnable(GL_TEXTURE_2D);

  createScene(SCENE_IDX);

  // main loop of progam
  bool windowIsOpen = true;
  while (windowIsOpen) {

    while (window.pollEvent(evnt)) { // check if program should close
      if (sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) windowIsOpen = false;
    }

    mainLoop(evnt);

    window.display();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    framerate(FPS, true);
  }

  return 0;
}

void mainLoop(sf::Event evnt) {

  updateScene(SCENE_IDX);

  static bool inside = true;

  // pause if P pressed
  static KeyDetector keyP(sf::Keyboard::P);
  if (keyP.typed()) {
    inside = !inside;
    window.setMouseCursorVisible(!inside);
    window.setMouseCursorGrabbed(inside);
  }

  // get position of mouse in window
  sf::Vector2i mousePos = sf::Mouse::getPosition(window);
  if (inside) {

    // move mouse back to centre of screen
    sf::Mouse::setPosition(sf::Vector2i(WIDTH/2, HEIGHT/2), window);

    // change camera orientation appropriately
    view.moveCamera((mousePos.x - WIDTH / 2) * 0.002, (mousePos.y - HEIGHT / 2) * 0.002);
  }

  // update the position of the camera
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) view.moveForward(3);
    else view.moveForward(1);
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) view.moveForward(-1);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) view.moveRight(-1);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) view.moveRight(1);

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) view.moveUp(1);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) view.moveUp(-1);

  // light intensity
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) scene.attenuation *= 1.01;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::N)) scene.attenuation *= 0.99;

  // add light
  static KeyDetector keyY(sf::Keyboard::Y);
  if (keyY.typed()) scene.addObject(PointLight(view.pos));

  //if (sf::Mouse::isButtonPressed(sf::Mouse::Middle) && scene.dirLights.empty()) scene.addObject(DirectionalLight(vec(0.5, -1, 1)));

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add) && !scene.dirLights.empty()) scene.dirLights[0].dir = scene.dirLights[0].dir.rotateAbout(vec(1, -1, 0), -0.05);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract) && !scene.dirLights.empty()) scene.dirLights[0].dir = scene.dirLights[0].dir.rotateAbout(vec(1, -1, 0), 0.05);

  //if (!scene.dirLights.empty()) {
  //  float f = vec(0, -1, 0).dot(scene.dirLights[0].dir);
  //  if (f > 0) scene.voidColor = vec(0, 0.7, 1) * f;
  //}

  scene.drawScene(view);
}

//// __ SCENE 1 __ ////
void createScene1() {

  // square
  scene.addObject(cuboid(vec(-20, 160, -20), vec(40, 0, 0), vec(0, 40, 0), vec(0, 0, 40), vec(0, 0.7, 0.7)));
  //scene.addObject(cuboid(vec(-120, 80, -120), vec(40, 0, 0), vec(0, 40, 0), vec(0, 0, 40), vec(0, 0.7, 0.7)));
  //scene.addObject(cuboid(vec(-60, 160, -60), vec(40, 0, 0), vec(0, 40, 0), vec(0, 0, 70), vec(0.7, 0.4, 0)));

  for (int i = 0; i < 20; i++) {
    scene.addObject(Sphere(vec(70 * sin(i * 2 * PI / 20), 0, 70 * cos(i * 2 * PI / 20)), 10, vec(abs(cos(i * 2 * PI / 20)), abs(cos(i * 2 * PI / 20)), abs(sin(i * 2 * PI / 20)))));
  }

  scene.addObject(Sphere(vec(0, 250, 0), 35, vec(1, 1, 1)));

  for (int i = 0; i < 30; i++) {
    scene.addObject(Sphere(vec(rand() % 400 - 200, rand() % 400, rand() % 400 - 200), rand() % 15 + 5, vec((rand() % 1000) / 1000.f, (rand() % 1000) / 1000.f, (rand() % 1000) / 1000.f)));
  }

  scene.addObject(PointLight(vec(0, 30, 1), vec(1, 0, 0)));
  scene.addObject(PointLight(vec(cos(2 * PI / 3), 30, sin(2 * PI / 3)), vec(0, 1, 0)));
  scene.addObject(PointLight(vec(cos(4 * PI / 3), 30, sin(4 * PI / 3)), vec(0, 0, 1)));

  // walls
  //scene.addObject(cuboid(vec(-200, -20, -200), vec(400, 0, 0), vec(0, 420, 0), vec(0, 0, 400), vec(0.3, 0.3, 0.3)));
}

void updateScene1() {

  static float theta = 0;
  theta += 0.01;

  scene.rotateObj(0, 0.02 * cos(theta), 0.02 * -sin(theta), 0.02 * sin(theta / 3));
  //scene.rotateObj(1, 0.02 * cos(theta), 0.02 * sin(theta), 0.02 * sin(theta / 3));
  //scene.rotateObj(2, 0, 0.02 * sin(theta), 0.02 * sin(theta));

  int y = 250;
  for (int i = 0; i < 20; i++) {
    scene.spheres[i].pos.y = y/2 * (cos(theta * 2 + 2 * i * PI / 5)) + y/2;
    //scene.spheres[i].pos.x = 70 * (cos(theta * 4 + 2 * i * PI / 20));
    //scene.spheres[i].pos.z = 70 * (sin(theta * 4 + 2 * i * PI / 20));
  }

  float d = 100 * cos(theta);
  scene.pointLights[0].pos = vec(d * cos(0 * PI / 3 + 2 * theta), 30, d * sin(0 * PI / 3 + 2 * theta));
  scene.pointLights[1].pos = vec(d * cos(2 * PI / 3 + 2 * theta), 30, d * sin(2 * PI / 3 + 2 * theta));
  scene.pointLights[2].pos = vec(d * cos(4 * PI / 3 + 2 * theta), 30, d * sin(4 * PI / 3 + 2 * theta));
}

//// __ SCENE 2 __ ////
void createScene2() {

  //scene.addObject(PointLight(vec(0, 100, 0)));

  scene.addObject(Sphere(vec(0, 0, 0), 1, vec(1, 0, 0)));
  scene.addObject(Sphere(vec(0, 0, 20), 1, vec(0, 0, 1)));
  scene.addObject(Sphere(vec(20, 0, 0), 1, vec(0, 1, 0)));

  int h = 120;
  int d = 5;

  scene.addObject(cuboid(vec(-100, 0, -200), vec(50, 0, 0), vec(10, h, 0), vec(0, 0, 50), vec(0.4, 0.4, 0)));
  scene.addObject(cuboid(vec( 50, 0, -200), vec(50, 0, 0), vec(-10, h, 0), vec(0, 0, 50), vec(0.4, 0.4, 0)));
  scene.addObject(cuboid(vec(-100-d, h, -200-d), vec(200+2*d, 0, 0), vec(0, 50, 0), vec(0, 0, 50+2*d), vec(0.3, 0.4, 0.2)));

  //scene.addObject(cuboid(vec(-100, 0, 150), vec(50, 0, 0), vec(10, h, 0), vec(0, 0, 50), vec(0.4, 0.4, 0)));
  //scene.addObject(cuboid(vec(50, 0, 150), vec(50, 0, 0), vec(-10, h, 0), vec(0, 0, 50), vec(0.4, 0.4, 0)));
  //scene.addObject(cuboid(vec(-100 - d, h, 150 - d), vec(200 + 2 * d, 0, 0), vec(0, 50, 0), vec(0, 0, 50 + 2 * d), vec(0.3, 0.4, 0.2)));

  //scene.addObject(Triangle(vec(-50, 0, 175), vec(50, 0, 175), vec(50, 150, 175), vec(0, 0, 0)));
  //scene.addObject(Triangle(vec(-50, 0, 175), vec(-50, 150, 175), vec(50, 150, 175), vec(0, 0, 0)));

  //scene.addObject(Triangle(vec(-50, 0, -175), vec(50, 0, -175), vec(50, 150, -175), vec(0, 0, 0)));
  //scene.addObject(Triangle(vec(-50, 0, -175), vec(-50, 150, -175), vec(50, 150, -175), vec(0, 0, 0)));

  scene.addObject(Sphere(vec(160, 160, 100), 75, vec(0.5, 0.5, 0.5)));
}

void updateScene2() {

}

void createScene(int i) {
  if (i == 1) createScene1();
  if (i == 2) createScene2();
}

void updateScene(int i) {
  if (i == 1) updateScene1();
  if (i == 2) updateScene2();
}