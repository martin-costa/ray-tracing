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

  launch();

  // main loop of progam
  bool windowIsOpen = true;
  while (windowIsOpen) {

    sf::Event evnt;
    while (window.pollEvent(evnt)) { // check if program should close
      if (sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) windowIsOpen = false;
    }

    mainLoop();

    window.display();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    framerate(FPS, true);
  }

  return 0;
}

void mainLoop() {

  //// __ automatic scene updates __ ////

  static float theta = 0;
  theta += 0.01;

  int y = 100;
  for (int i = 0; i < 12; i++) {
    for (int j = 0; j < 3; j++) {
      scene.triangles[i].p[j] = (scene.triangles[i].p[j] - vec(0, y, 0)).rotX(0.02 * sin(theta)) + vec(0, y, 0);
      scene.triangles[i].p[j] = (scene.triangles[i].p[j] - vec(0, y, 0)).rotY(0.02 * cos(theta * 1.1)) + vec(0, y, 0);
      scene.triangles[i].p[j] = (scene.triangles[i].p[j] - vec(0, y, 0)).rotZ(0.02 * sin(theta * 0.8)) + vec(0, y, 0);
    }
  }

  for (int i = 1; i <= 20; i++) {
    scene.spheres[i].pos.y = 30 * (cos(theta * 2 + 2 *i * PI / 20)) + y;

    scene.spheres[i].pos.x = 70 * (cos(theta * 4 + 2 * i * PI / 20));
    scene.spheres[i].pos.z = 70 * (sin(theta * 4 + 2 * i * PI / 20));
  }

  float d = 100 * cos(theta);
  scene.pointLights[0].pos = vec(d * cos(0 * PI / 3 + 2 * theta), 30, d * sin(0 * PI / 3 + 2 * theta));
  scene.pointLights[1].pos = vec(d * cos(2 * PI / 3 + 2 * theta), 30, d * sin(2 * PI / 3 + 2 * theta));
  scene.pointLights[2].pos = vec(d * cos(4 * PI / 3 + 2 * theta), 30, d * sin(4 * PI / 3 + 2 * theta));

  static bool inside = true;

  //// __ inputs __ ////

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

  // light insenity
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) scene.attenuation *= 1.01;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::N)) scene.attenuation *= 0.99;

  // add light
  static KeyDetector keyY(sf::Keyboard::Y);
  if (keyY.typed()) scene.addObject(PointLight(view.pos));

  scene.drawScene(view);
}

void launch() {

  rand(); // dont like base

  //// SCENE 1
  scene.addObject(Sphere(vec(0, 200, 0), 35, vec(1, 1, 1)));
  for (int i = 0; i < 20; i++) {
    scene.addObject(Sphere(vec(70 * sin(i * 2 * PI / 20), 0, 70 * cos(i * 2 * PI / 20)), 10, vec(abs(cos(i * 2 * PI / 20)), abs(cos(i * 2 * PI / 20)), abs(sin(i * 2 * PI / 20)))));
  }
  for (int i = 0; i < 50; i++) {
    scene.addObject(Sphere(vec(rand() % 400 - 200, rand() % 400, rand() % 400 - 200), rand() % 15 + 5, vec((rand() % 1000) / 1000.f, (rand() % 1000) / 1000.f, (rand() % 1000) / 1000.f)));
  }

  // square
  int y = 100;
  vec verts[] = { vec(-20, y - 20, -20), vec(-20, y - 20, 20), vec(20, y - 20, 20), vec(20, y - 20, -20),
                  vec(-20, y + 20, -20), vec(-20, y + 20, 20), vec(20, y + 20, 20), vec(20, y + 20, -20) };

  int indices[] = { 0,1,3,  2,1,3,  4,5,6,  4,7,6,  0,3,7,  0,4,7,  1,2,6,  1,5,6,  3,2,6,  3,6,7,  0,1,5,  0,4,5 };
  scene.addObject( TriangleArray(verts, 8, indices, 36, vec(0, 0.7, 0.7)) );

  scene.addObject(PointLight(vec(0, 30, 1), vec(1, 0, 0)));
  scene.addObject(PointLight(vec(cos(2 * PI / 3), 30, sin(2 * PI / 3)), vec(0, 1, 0)));
  scene.addObject(PointLight(vec(cos(4 * PI / 3), 30, sin(4 * PI / 3)), vec(0, 0, 1)));

  // box
  //vec verts1[] = { vec(-200, y - 200, -200), vec(-200, y - 200, 200), vec(200, y - 200, 200), vec(200, y - 200, -200),
  //                vec(-200, y + 200, -200), vec(-200, y + 200, 200), vec(200, y + 200, 200), vec(200, y + 200, -200) };
  //int indices1[] = { 4,5,6,  4,7,6,  0,3,7,  0,4,7,  1,2,6,  1,5,6,  3,2,6,  3,6,7,  0,1,5,  0,4,5 };
  //scene.addObject(TriangleArray(verts1, 8, indices1, 30, vec(0.7, 0.7, 0.7)));
  
  // box of spheres
  //for (int i = 0; i < 5; i++) {
  //  for (int j = 0; j < 5; j++) {
  //    for (int k = 0; k < 5; k++) {
  //      scene.addObject(Sphere(vec(10 * i, 10*k + 75, 10 * j), 5, vec(i*0.2, j*0.2, k*0.2)));
  //    }
  //  }
  //}

}