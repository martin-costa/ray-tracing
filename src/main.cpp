#include "main.hpp"

typedef Vector3d vec;

int main() {

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

  // SCENE 1
  scene.addObject(Sphere(vec(0, 100, 0), 35, vec(1, 1, 1)));
  for (int i = 0; i < 20; i++) {
    scene.addObject(Sphere(vec(70 * sin(i * 2 * PI / 20), i*10, 70 * cos(i * 2 * PI / 20)), 10, vec(abs(cos(i * 2 * PI / 20)), abs(cos(i * 2 * PI / 20)), abs(sin(i * 2 * PI / 20)))));
  }
  for (int i = 0; i < 100; i++) {
    scene.addObject(Sphere(vec(rand() % 800 - 400, rand() % 800, rand() % 800 - 400), rand() % 15 + 5, vec((rand() % 1000) / 1000.f, (rand() % 1000) / 1000.f, (rand() % 1000) / 1000.f)));
  }
  scene.addObject(Sphere(vec(0, 180, 20), 15, vec(1, 0, 1)));

  int y = 300;
  vec verts[] = { vec(-50, y-50, -50), vec(-50, y-50, 50), vec(50, y-50, 50), vec(50, y-50, -50),
                  vec(-50, y+50, -50), vec(-50, y+50, 50), vec(50, y+50, 50), vec(50, y+50, -50) };

  scene.addObject(Triangle(verts[0], verts[1], verts[3], vec(0, 0.7, 0.7)));
  scene.addObject(Triangle(verts[2], verts[1], verts[3], vec(0, 0.7, 0.7)));

  scene.addObject(Triangle(verts[4], verts[5], verts[6], vec(0, 0.7, 0.7)));
  scene.addObject(Triangle(verts[4], verts[7], verts[6], vec(0, 0.7, 0.7)));

  scene.addObject(Triangle(verts[0], verts[3], verts[7], vec(0, 0.7, 0.7)));
  scene.addObject(Triangle(verts[0], verts[4], verts[7], vec(0, 0.7, 0.7)));

  scene.addObject(Triangle(verts[1], verts[2], verts[6], vec(0, 0.7, 0.7)));
  scene.addObject(Triangle(verts[1], verts[5], verts[6], vec(0, 0.7, 0.7)));

  scene.addObject(Triangle(verts[3], verts[2], verts[6], vec(0, 0.7, 0.7)));
  scene.addObject(Triangle(verts[3], verts[6], verts[7], vec(0, 0.7, 0.7)));

  scene.addObject(Triangle(verts[0], verts[1], verts[5], vec(0, 0.7, 0.7)));
  scene.addObject(Triangle(verts[0], verts[4], verts[5], vec(0, 0.7, 0.7)));

  // main loop of progam
  bool windowIsOpen = true;
  while (windowIsOpen) {

    sf::Event evnt;
    while (window.pollEvent(evnt)) { // check if program should close
      if (sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) windowIsOpen = false;
    }

    for (int i = 0; i < 12; i++) {
      scene.triangles[i].p1 = (scene.triangles[i].p1 - vec(0, y, 0)).rotateAbout(vec(1, 1, 1).normalize(), 0.01) + vec(0, y, 0);
      scene.triangles[i].p2 = (scene.triangles[i].p2 - vec(0, y, 0)).rotateAbout(vec(1, 1, 1).normalize(), 0.01) + vec(0, y, 0);
      scene.triangles[i].p3 = (scene.triangles[i].p3 - vec(0, y, 0)).rotateAbout(vec(1, 1, 1).normalize(), 0.01) + vec(0, y, 0);
    }

    static float theta = 0;
    theta += 0.01;

    scene.spheres[0].pos.y = sin(theta) * 100 + 100;

    mainLoop();

    window.display();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    framerate(FPS, false);
  }

  return 0;
}

void mainLoop() {

  static bool inside = true;

  // pause if P pressedp
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

  scene.drawScene(view);
}