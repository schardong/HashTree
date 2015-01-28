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

QuadTree* qt;

typedef struct point_t
{
  vec2 p;
  vec3 c;
} q_point;

vector<q_point> points;

void initGL()
{
  array<vec2, 4> v0 = {vec2(0, 0.1), vec2(0.8, 0.1), vec2(1, 0.9), vec2(0.2, 0.9)};
  Rhombus* r0 = new Rhombus(v0);
  qt = new QuadTree(r0, RHOMBUS, 2);

  qt->AddPoint(vec2(0.3, 0.3));
  qt->AddPoint(vec2(0.3, 0.7));
  qt->AddPoint(vec2(0.4, 0.7));
  QuadTreeNode* test_node = qt->GetRoot()->GetChild(2)->FindNeighbor(N);

  glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glPointSize(2);
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);

  glBegin(GL_POINTS);
  for(int i = 0; i < points.size(); ++i) {
    glColor3f(points[i].c.x, points[i].c.y, points[i].c.z);
    glVertex2f(points[i].p.x, points[i].p.y);
  }
  glEnd();

  glColor3f(1, 0, 0);

  qt->draw();

  glFlush();
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

void mouseClick(int button, int state, int x, int y)
{
  if(state == GLUT_UP) {
    switch(button)
    {
    case GLUT_LEFT_BUTTON:
    default:
      q_point p;
      p.p = vec2(x / (float)WIN_WIDTH, (WIN_HEIGHT - y) / (float)WIN_HEIGHT);
      if(qt->AddPoint(p.p)) {
        p.c = vec3(0, 1, 0);
        points.push_back(p);
      }
      break;
    }
  }
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
  glutInitWindowPosition(50, 50);
  glutCreateWindow("Viewport Transform");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouseClick);
  initGL();
  glutMainLoop();
  return 0;
}
