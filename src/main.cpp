#include <iostream>
#include <glm/glm.hpp>
#include <array>
#include <vector>
#include <map>
#include <fstream>

#include "quadtree.h"
#include "edge.h"
//#include "faille4.h"
#include "opencv2/opencv.hpp"

#include <GL/glut.h>

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

GLuint tex_id;

void createTree()
{
  array<vec2, 4> v0 = {vec2(0, 0.1), vec2(0.8, 0.1), vec2(1, 0.9), vec2(0.2, 0.9)};

  array<vec2, 4> v1 = {vec2(LEFT, BOTTOM), vec2(RIGHT, BOTTOM), vec2(RIGHT, TOP), vec2(LEFT, TOP)};

  BBox* r0 = new BBox(v1);
  

  Mat I;
  I = imread("faille4.bmp", CV_LOAD_IMAGE_GRAYSCALE);
  if(!I.data) {
    cerr << "No image loaded.\n";
  }

  //glGenTextures(1, &tex_id);
  //glBindTexture(GL_TEXTURE_2D, tex_id);
  //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // Linear Filtering
  //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // Linear Filtering
  //glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, I.cols, I.rows, 0, GL_R, GL_UNSIGNED_BYTE, I.data);
  
  CV_Assert(I.depth() != sizeof(uchar));

  int channels = I.channels();

  int nRows = I.rows;
  int nCols = I.cols * channels;

  int i,j;
  uchar* p;
  for( i = 0; i < nRows; ++i) {
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

  qt = new QuadTree(r0, 32);//, -1, points);
  for(size_t i = 0; i < points.size(); ++i) {
    qt->AddPoint(points[i]);
  }

  printf("%d %d %d %d\n", qt->GetLeaves(0).size(), qt->GetLeaves(1).size(), qt->GetLeaves(2).size(), qt->GetLeaves(3).size());


  /*for(int i = 0; i < 24; i += 3) {
    printf("(%d %d %d)\n", gimp_image[i], gimp_image[i+1], gimp_image[i+2]);
  }*/

  
  /*for(int i = 0; i < h; ++i) {
    for(int j = 0; j < w; ++j) {
      if(input.data
    }
  }*/

  //    if(bmp_data[i * w + j] == 0) {
  //      float x = j / (float)w;
  //      float y = i / (float)h;
  //      Vertex* v = new Vertex(vec2(x, y));
  //      if(qt->AddPoint(v))
  //        points.push_back(v);
  //      else
  //        delete v;
  //    }
  //  }
  //}
  //
  //delete bmp;
  //bmp = nullptr;

  //memset(bmp_data, 0, sizeof(uchar) * w * h);
  //delete bmp_data;
  //bmp_data = nullptr;
}

void initGL()
{
  createTree();
  glEnable(GL_TEXTURE_2D);
  
  glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glMatrixMode(GL_MODELVIEW);
  
  glLoadIdentity();
  glPointSize(1);
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);

  /*glBindTexture(GL_TEXTURE_2D, tex_id);

  glColor3f(1, 0, 1);
  glBegin(GL_QUADS);

  glTexCoord2f(0.f, 0.f);
  glVertex2f(0.f, 0.f);

  glTexCoord2f(0.f, 1.f);
  glVertex2f(0.f, 1.f);

  glTexCoord2f(1.f, 1.f);
  glVertex2f(1.f, 1.f);

  glTexCoord2f(1.f, 0.f);
  glVertex2f(1.f, 0.f);

  glEnd();*/

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
  size_t sz = points.size();

  switch(c) {
  case GLUT_KEY_F1:
    balance_tree(qt);
    break;
  case GLUT_KEY_F2:
    /*for(size_t i = 0; i < sz; ++i)
      points[i]->edges.clear();

    for(size_t i = 0; i < edges.size(); ++i) {
      memset(edges[i], 0, sizeof(Edge));
      delete edges[i];
      edges[i] = nullptr;
    }
    edges.clear();

    for(size_t i = 1; i <= sz; ++i) {
      size_t idx1 = (i-1) % sz;
      size_t idx2 = i % sz;
      Edge* e1 = new Edge(points[idx1], points[idx2]);
      points[idx1]->edges.push_back(e1);
      edges.push_back(e1);
    }

    for(size_t i = 1; i <= sz; ++i) {
      size_t idx1 = (i-1) % sz;
      size_t idx2 = i % sz;
      Edge* e1 = new Edge(points[idx2], points[idx1]);
      points[idx2]->edges.push_back(e1);
      edges.push_back(e1);
    }

    enforce_corners(qt);*/
    break;
  case GLUT_KEY_F3:
    //find and delete empty nodes.
    //delete_out_nodes(qt, points);
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
  //glutIdleFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse_click);
  glutKeyboardFunc(key_press);
  glutSpecialFunc(key_press_special);

  initGL();
  glutMainLoop();
  return 0;
}
