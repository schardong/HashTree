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

vector<vec2> points;

void initGL()
{
  array<vec2, 4> v0 = {vec2(0, 0.1), vec2(0.8, 0.1), vec2(1, 0.9), vec2(0.2, 0.9)};
  Rhombus* r0 = new Rhombus(v0);
  qt = new QuadTree(r0, RHOMBUS, 2);


  points.push_back(vec2(0.3, 0.3));
  points.push_back(vec2(0.3, 0.8));
  points.push_back(vec2(0.4, 0.8));
  points.push_back(vec2(0.5, 0.4));
  points.push_back(vec2(0.6, 0.4));
  points.push_back(vec2(0.7, 0.4));
  points.push_back(vec2(0.8, 0.4));

  points.push_back(vec2(0.7, 0.6));
  points.push_back(vec2(0.75, 0.6));
  points.push_back(vec2(0.8, 0.8));
  points.push_back(vec2(0.85, 0.8));

  for(size_t i = 0; i < points.size(); ++i)
    qt->AddPoint(points[i]);

  QuadTreeNode* test_node0 = qt->GetRoot()->GetChild(3)->GetChild(0)->FindNeighbor(S);
  QuadTreeNode* test_node1 = qt->GetRoot()->GetChild(3)->GetChild(1)->FindNeighbor(S);
  QuadTreeNode* test_node2 = qt->GetRoot()->GetChild(1)->GetChild(2)->FindNeighbor(N);
  QuadTreeNode* test_node3 = qt->GetRoot()->GetChild(1)->GetChild(3)->FindNeighbor(N);

  if(test_node0)
    cout << test_node0->GetId() << endl;
  if(test_node1)
    cout << test_node1->GetId() << endl;
  if(test_node2)
    cout << test_node2->GetId() << endl;
  if(test_node3)
    cout << test_node3->GetId() << endl;

  glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glPointSize(2);
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(0, 1, 0);
  glBegin(GL_POINTS);
  for(size_t i = 0; i < points.size(); ++i)
    glVertex2f(points[i].x, points[i].y);
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
      vec2 p;
      p = vec2(x / (float)WIN_WIDTH, (WIN_HEIGHT - y) / (float)WIN_HEIGHT);
      if(qt->AddPoint(p))
        points.push_back(p);
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
