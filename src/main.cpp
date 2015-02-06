#include <iostream>
#include <glm/glm.hpp>
#include <array>
#include <vector>
#include <map>

#include "quadtree.h"
#include "edge.h"

#include <GL/glut.h>

using namespace std;
using namespace glm;

static int WIN_WIDTH = 800;
static int WIN_HEIGHT = 800;
static int win_id = -1;

QuadTree* qt;

vector<Vertex*> points;
vector<Edge*> edges;
int g_vertex_id = 0;

void initGL()
{
  array<vec2, 4> v0 = {vec2(0, 0.1), vec2(0.8, 0.1), vec2(1, 0.9), vec2(0.2, 0.9)};
  BBox* r0 = new BBox(v0);
  qt = new QuadTree(r0, 1);

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
    glVertex2f(points[i]->p.x, points[i]->p.y);
  glEnd();

  glColor3f(0, 1, 1);

  glBegin(GL_LINES);
  for(size_t i = 0; i < edges.size(); ++i) {
    glVertex2f(edges[i]->u->p.x, edges[i]->u->p.y);
    glVertex2f(edges[i]->v->p.x, edges[i]->v->p.y);
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
      Vertex* v = new Vertex(vec2(x / (float)WIN_WIDTH, (WIN_HEIGHT - y) / (float)WIN_HEIGHT));
      v->id = g_vertex_id++;
      if(qt->AddPoint(v))
        points.push_back(v);
      break;
    }
  }
}

void key_press(unsigned char c, int, int)
{
  printf("%d %c\n", c, c);

  switch(c) {
  case 27: //ESC
    glutDestroyWindow(win_id);
    delete qt;
    qt = nullptr;

    for(size_t i = 0; i < points.size(); ++i) {
      memset(points[i], 0, sizeof(Vertex));
      delete points[i];
      points[i] = nullptr;
    }

    for(size_t i = 0; i < edges.size(); ++i) {
      memset(edges[i], 0, sizeof(Edge));
      delete edges[i];
      edges[i] = nullptr;
    }

    points.clear();
    edges.clear();

    exit(0);
    break;
  case 32: //SPACEBAR
    break;
  }

  glutPostRedisplay();
}

void key_press_special(int c, int, int)
{
  printf("%d %c\n", c, c);

  switch(c) {
  case GLUT_KEY_F1:
    balance_tree(qt);
    break;
  case GLUT_KEY_F2:
    for(size_t i = 0; i < points.size(); ++i) {
      points[i]->edges.clear();
    }

    for(size_t i = 0; i < edges.size(); ++i) {
      memset(edges[i], 0, sizeof(Edge));
      delete edges[i];
      edges[i] = nullptr;
    }
    edges.clear();
    
    size_t sz = points.size();
    for(size_t i = 1; i <= points.size(); ++i) {
      size_t idx1 = (i-1) % sz;
      size_t idx2 = i % sz;
      Edge* e1 = new Edge(points[idx1], points[idx2]);
      points[idx1]->edges.push_back(e1);
      edges.push_back(e1);
    }

    for(size_t i = 1; i <= points.size(); ++i) {
      size_t idx1 = (i-1) % sz;
      size_t idx2 = i % sz;
      Edge* e1 = new Edge(points[idx2], points[idx1]);
      points[idx2]->edges.push_back(e1);
      edges.push_back(e1);
    }

    enforce_corners(qt);
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
  //ADD-THIS-LATER!
  glutIdleFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse_click);
  glutKeyboardFunc(key_press);
  glutSpecialFunc(key_press_special);

  initGL();
  glutMainLoop();
  return 0;
}
