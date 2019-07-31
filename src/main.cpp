#include "main.hpp"

int main() {

  //create the window for opengl
  window.create(sf::VideoMode(WIDTH, HEIGHT), "Ray Tracer", sf::Style::Titlebar);
  window.setMouseCursorVisible(false);
  window.setMouseCursorGrabbed(true);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  //set normal coord system
  glScalef(1.0f, -1.0f, 1.0f);
  glOrtho(0, WIDTH, HEIGHT, 0, 0, 1);
  glTranslatef(WIDTH / 2, HEIGHT / 2, 0.0f);
  glMatrixMode(GL_MODELVIEW);

  //scene.addObject(Sphere(Vector3d(0, 0, 0), 1000, Vector3d(0, 1, 1)));

  scene.addObject(Sphere(Vector3d(0, 0, 50), 10, Vector3d(1, 0, 0)));


  for (int i = 0; i < 329; i++) {
    scene.addObject(Sphere(Vector3d(rand() % 800 - 400, rand() % 800 - 400, rand() % 800 - 400), rand() % 15 + 5, Vector3d(1000.0f/(rand() % 1000), 1000.0f / (rand() % 1000), 1000.0f / (rand() % 1000))));
  }

  //main loop of progam
  bool windowIsOpen = true;
  while (windowIsOpen) {

    sf::Event evnt;
    while (window.pollEvent(evnt)) { //check if program should close
      if (sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) windowIsOpen = false;
    }

    mainLoop();

    window.display();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0.1, 1);

    framerate(FPS, true);
  }

  return 0;
}

void mainLoop() {

  static bool inside = true;

  //pause if P pressed
  static KeyDetector keyP(sf::Keyboard::P);
  if (keyP.typed()) {
    inside = !inside;
    window.setMouseCursorVisible(!inside);
    window.setMouseCursorGrabbed(inside);
  }

  //toggle lights
  static KeyDetector keyT(sf::Keyboard::T);
  if (keyT.typed()) {
    scene.light = !scene.light;
  }

  //get position of mouse in window
  sf::Vector2i mousePos = sf::Mouse::getPosition(window);
  if (inside) {

    //move mouse back to centre of screen
    sf::Mouse::setPosition(sf::Vector2i(WIDTH/2, HEIGHT/2), window);

    //change camera orientation appropriately
    scene.view.moveCamera((mousePos.x - WIDTH / 2) * 0.002, (mousePos.y - HEIGHT / 2) * 0.002);
  }

  //update the position of the camera
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) scene.view.moveRight(1);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) scene.view.moveRight(-1);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) scene.view.moveForward(1);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) scene.view.moveForward(-1);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) scene.view.moveUp(1);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) scene.view.moveUp(-1);

  scene.drawScene();
}