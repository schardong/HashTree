#include <iostream>
#include <glm/glm.hpp>
#include <array>
#include <vector>

#include "quadtree.h"

#include <GL/glut.h>

using std::cout;
using std::endl;
using std::array;
using std::vector;
using glm::vec2;
using glm::vec3;

static int WIN_WIDTH = 800;
static int WIN_HEIGHT = 800;
static int win_id = -1;

QuadTree* qt;

vector<vec2> points;

void initGL()
{
  array<vec2, 4> v0 = {vec2(0, 0.1), vec2(0.8, 0.1), vec2(1, 0.9), vec2(0.2, 0.9)};
  Rhombus* r0 = new Rhombus(v0);
  qt = new QuadTree(r0, RHOMBUS, 1);

//  points.push_back(vec2(0.3, 0.3));
//  points.push_back(vec2(0.3, 0.8));
//  points.push_back(vec2(0.4, 0.8));
//  points.push_back(vec2(0.5, 0.4));
//  points.push_back(vec2(0.6, 0.4));
//  points.push_back(vec2(0.7, 0.4));
//  points.push_back(vec2(0.8, 0.4));

//  points.push_back(vec2(0.7, 0.6));
//  points.push_back(vec2(0.75, 0.6));
//  points.push_back(vec2(0.8, 0.8));
//  points.push_back(vec2(0.85, 0.8));

//  for(size_t i = 0; i < points.size(); ++i)
//    qt->AddPoint(points[i]);

  glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glPointSize(3);
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(0, 1, 0);
  glBegin(GL_POINTS);
  for(size_t i = 0; i < points.size(); ++i)
    glVertex2f(points[i].x, points[i].y);
  glEnd();

  glColor3f(0, 1, 1);
  glBegin(GL_LINE_LOOP);
  for(size_t i = 0; i < points.size(); ++i) {
    glVertex2f(points[i].x, points[i].y);
  }
  glEnd();

  glColor3f(1, 0, 0);
  qt->draw();

  glutSwapBuffers();
}

void reshape(GLsizei width, GLsizei height)
{
  if(height == 0)
    height = 1;

  WIN_WIDTH = width;
  WIN_HEIGHT = height;

  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluOrtho2D(0, 1, 0, 1);
}

void mouse_click(int button, int state, int x, int y)
{
  if(state == GLUT_UP) {
    switch(button)
    {
    case GLUT_LEFT_BUTTON:
    default:
      vec2 p;
      p = vec2(x / (float)WIN_WIDTH, (WIN_HEIGHT - y) / (float)WIN_HEIGHT);
      if(qt->AddPoint(p))
        points.push_back(p);
      break;
    }
  }
}

void keyboard_down(unsigned char c, int, int)
{
  switch(c) {
  case 27: //ESC
    glutDestroyWindow(win_id);
    delete qt;
    qt = nullptr;
    exit(0);
    break;
  case 32: //SPACEBAR
    cout << "balancing!\n";
    qt->BalanceTree();
    break;
  case GLUT_KEY_F1:
    qt->EnforceCornerCond();
    break;
  }

  glutPostRedisplay();
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
  glutInitWindowPosition(50, 50);
  win_id = glutCreateWindow("Viewport Transform");
  glutDisplayFunc(display);
  glutIdleFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse_click);
  glutKeyboardFunc(keyboard_down);

  initGL();
  glutMainLoop();
  return 0;
}
