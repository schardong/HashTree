#include <iostream>
#include <glm/glm.hpp>
#include <array>
#include <vector>
#include <map>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <GL/glut.h>

#include "quadtree.h"
#include "edge.h"
#include "opencv2/opencv.hpp"

using namespace std;
using namespace glm;
using namespace cv;

static int WIN_WIDTH = 512;
static int WIN_HEIGHT = 512;
static int win_id = -1;

QuadTree* qt;

vector<Vertex*> points;
vector<Edge*> edges;
int g_vertex_id = 0;

#define LEFT 0.0001f
#define RIGHT 1.f
#define TOP 1.f
#define BOTTOM 0.0001f

void createTree()
{
  array<vec2, 4> v0 = {vec2(LEFT, BOTTOM), vec2(RIGHT, BOTTOM), vec2(RIGHT, TOP), vec2(LEFT, TOP)};
  BBox* r0 = new BBox(v0);

  Mat I = imread("faille4.bmp", CV_LOAD_IMAGE_GRAYSCALE);
  if(!I.data) {
    cerr << "No image loaded.\n";
  }

  CV_Assert(I.depth() != sizeof(uchar));

  int channels = I.channels();

  int nRows = I.rows;
  int nCols = I.cols * channels;

  int i,j;
  uchar* p;
  for(i = 0; i < nRows; ++i) {
    p = I.ptr<uchar>(i);
    for ( j = 0; j < nCols; ++j) {
      if(p[j] == 0) {
        float x = j / (float)nRows + BOTTOM;
        float y = i / (float)nCols + LEFT;
        Vertex* v = new Vertex(vec2(x, y));
        if(r0->PointInBox(vec2(x, y)))
          points.push_back(v);
        else
          delete v;
      }
    }
  }

  qt = new QuadTree(r0, 32, 4);//, -1, points);
  for(size_t i = 0; i < points.size(); ++i)
    qt->AddPoint(points[i]);
}

void initGL()
{
  createTree();
  glEnable(GL_TEXTURE_2D);
  
  glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glMatrixMode(GL_MODELVIEW);
  
  glLoadIdentity();
  glPointSize(1);
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
  gluOrtho2D(LEFT, RIGHT, BOTTOM, TOP);

  glutPostRedisplay();
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

  glutPostRedisplay();
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
  }

  glutPostRedisplay();
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
  glutInitWindowPosition(50, 50);
  win_id = glutCreateWindow("Quadtree demo");
  glutDisplayFunc(display);
  //ADD-THIS-LATER!
  //glutIdleFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse_click);
  glutKeyboardFunc(key_press);
  glutSpecialFunc(key_press_special);

  initGL();
  glutMainLoop();
  return 0;
}
