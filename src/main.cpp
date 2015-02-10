#include <iostream>
#include <glm/glm.hpp>
#include <array>
#include <vector>
#include <map>
#include <fstream>

#include "quadtree.h"
#include "edge.h"
#include "bmp.h"

#include <GL/glut.h>

using namespace std;
using namespace glm;

static int WIN_WIDTH = 512;
static int WIN_HEIGHT = 512;
static int win_id = -1;

QuadTree* qt;

vector<Vertex*> points;
vector<Edge*> edges;
int g_vertex_id = 0;

#define LEFT 0.f
#define RIGHT 1.f
#define TOP 1.f
#define BOTTOM 0.f

GLuint tex_id;

int width = 0;
int height = 0;
short BitsPerPixel = 0;
std::vector<unsigned char> Pixels;

void LoadBMP(const char* FilePath)
{
    std::fstream hFile(FilePath, std::ios::in | std::ios::binary);
    if (!hFile.is_open()) throw std::invalid_argument("Error: File Not Found.");

    hFile.seekg(0, std::ios::end);
    int Length = hFile.tellg();
    hFile.seekg(0, std::ios::beg);
    std::vector<std::uint8_t> FileInfo(Length);
    hFile.read(reinterpret_cast<char*>(FileInfo.data()), 54);

    if(FileInfo[0] != 'B' && FileInfo[1] != 'M')
    {
        hFile.close();
        throw std::invalid_argument("Error: Invalid File Format. Bitmap Required.");
    }

    if (FileInfo[28] != 24 && FileInfo[28] != 32)
    {
        hFile.close();
        throw std::invalid_argument("Error: Invalid File Format. 24 or 32 bit Image Required.");
    }

    BitsPerPixel = FileInfo[28];
    width = FileInfo[18] + (FileInfo[19] << 8);
    height = FileInfo[22] + (FileInfo[23] << 8);
    std::uint32_t PixelsOffset = FileInfo[10] + (FileInfo[11] << 8);
    std::uint32_t size = ((width * BitsPerPixel + 31) / 32) * 4 * height;
    Pixels.resize(size);

    hFile.seekg (PixelsOffset, std::ios::beg);
    hFile.read(reinterpret_cast<char*>(Pixels.data()), size);
    hFile.close();
}

void initGL()
{
  array<vec2, 4> v0 = {vec2(0, 0.1), vec2(0.8, 0.1), vec2(1, 0.9), vec2(0.2, 0.9)};

  array<vec2, 4> v1 = {vec2(LEFT, BOTTOM), vec2(512, BOTTOM), vec2(512, 512), vec2(LEFT, 512)};

  BBox* r0 = new BBox(v1);
  qt = new QuadTree(r0, 128);

  //Bmp* bmp = new Bmp("faille4.bmp"); 
  /*Bmp* bmp = new Bmp("test_01.bmp"); 
  uchar* bmp_data = bmp->getImage();

  int w = bmp->getWidth();
  int h = bmp->getHeight();

  for(int i = 0; i < h; ++i) {
    for(int j = 0; j < w; ++j) {
      if(bmp_data[i * w + j] == 0) {
        float x = j / (float)w;
        float y = i / (float)h;
        Vertex* v = new Vertex(vec2(x, y));
        if(qt->AddPoint(v))
          points.push_back(v);
        else
          delete v;
      }
    }
  }*/

  LoadBMP("test_01.bmp");

  /*glGenTextures(1, &tex_id);
  glBindTexture(GL_TEXTURE_2D, tex_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, Pixels.data());*/

  int w = 512;
  int h = 512;

  for(int i = 0; i < h; ++i) {
    for(int j = 0; j < w; ++j) {
      if(Pixels.data()[i * w + j] == 0) {
        float x = j; // (float)w;
        float y = i; // (float)h;
        Vertex* v = new Vertex(vec2(x, y));
        if(qt->AddPoint(v))
          points.push_back(v);
        else
          delete v;
      }
    }
  }
  
  /*delete bmp;
  bmp = nullptr;

  memset(bmp_data, 0, sizeof(uchar) * w * h);
  delete bmp_data;
  bmp_data = nullptr;*/

  glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  /*glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/
  glMatrixMode(GL_MODELVIEW);
  glEnable(GL_TEXTURE_2D);
  glLoadIdentity();
  glPointSize(1);
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);

  /*glBindTexture(GL_TEXTURE_2D, tex_id);

  glColor3f(1, 1, 1);
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

  /*glBegin(GL_LINES);
  for(size_t i = 0; i < edges.size(); ++i) {
    glVertex2f(edges[i]->u->p.x, edges[i]->u->p.y);
    glVertex2f(edges[i]->v->p.x, edges[i]->v->p.y);
  }
  glEnd();*/

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
  gluOrtho2D(0, width, 0, height);

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
    for(size_t i = 0; i < sz; ++i)
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

    enforce_corners(qt);
    break;
  case GLUT_KEY_F3:
    //find and delete empty nodes.
    delete_out_nodes(qt, points);
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
