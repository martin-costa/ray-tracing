#include "main.hpp"

int main() {

  //create the window for opengl
  window.create(sf::VideoMode(WIDTH, HEIGHT), "Ray Tracer", sf::Style::Titlebar);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  //set normal coord system
  glOrtho(0, WIDTH, HEIGHT, 0, 0, 1);
  glMatrixMode(GL_MODELVIEW);

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
    glClearColor(0, 0, 0.2, 1);

    framerate(FPS, true);
  }

  return 0;
}

void mainLoop() {

  sf::Vector2i mouse = sf::Mouse::getPosition(window);

  glBegin(GL_POINTS);

  for (int i = -WIDTH/2; i < WIDTH/2; i++) {
    for (int j = -HEIGHT/2; j < HEIGHT/2; j++) {

      Vector3d c = sphere.color;

      double d = sphere.closestIntersection(Line(Vector3d(0, 0, -900), Vector3d(i, j, 900).normalize()));
      if (d > 0) {
        glColor3f(c.x, c.y, c.z);
        glVertex2i(i + WIDTH/2, j + HEIGHT/2);
      }

      c = sphere2.color;

      d = sphere2.closestIntersection(Line(Vector3d(0, 0, -900), Vector3d(i, j, 900).normalize()));
      if (d > 0) {
        glColor3f(c.x, c.y, c.z);
        glVertex2i(i + WIDTH / 2, j + HEIGHT / 2);
      }
    }
  }

  glEnd();
}