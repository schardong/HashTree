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
#include "mesh.h"
#include "CImg.h"

using namespace std;
using namespace glm;
using namespace cimg_library;

static int WIN_WIDTH = 512;
static int WIN_HEIGHT = 512;
static int win_id = -1;

QuadTree* qt = nullptr;
Mesh* g_mesh = nullptr;

vector<Vertex*> points;
vector<Edge*> edges;

#define LEFT 0.0001f
#define RIGHT 1.f
#define TOP 1.f
#define BOTTOM 0.0001f

void createTree()
{
	array<vec2, 4> v0 = { vec2(LEFT, BOTTOM), vec2(RIGHT, BOTTOM), vec2(RIGHT, TOP), vec2(LEFT, TOP) };
	BBox* r0 = new BBox(v0);

	//AXELLE
	CImg<unsigned char> image;
	image.load_bmp("faille4.bmp");
	int w = image.width();
	int h = image.height();
	for (int i = 0; i < w; i++){
		for (int j = 0; j < h; j++){
			if (image(i, j) == 0){ //black points
				float normi = (float)i / w;
				float normj = (float)j / h;
				Vertex* v = new Vertex(vec2(normi, 1-normj));
				if (r0->PointInBox(vec2(normi, 1-normj)))
					points.push_back(v);
				else
					delete v;
			}
		}
	}

  qt = new QuadTree(r0, 10, 4);//, -1, points);
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
  if (g_mesh == nullptr) {
	  qt->draw();
  }
  else
	  g_mesh->draw();

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
  gluOrtho2D(0.f, RIGHT, BOTTOM, TOP);

  glutPostRedisplay();
}

void mouse_click(int button, int state, int x, int y)
{
  if(state == GLUT_UP) {
    switch(button)
    {
    case GLUT_LEFT_BUTTON:
    default:
      Vertex* v = new Vertex(vec2(x / (float)WIN_WIDTH, y / (float)WIN_HEIGHT));
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
  case GLUT_KEY_F2:
    QuadTreeNode* node = qt->GetRoot()->GetChild(NE);
    vector<QuadTreeNode*> test = get_nbrs_vertex(node, qt->GetLeaves(), 3);
    for(size_t i = 0; i < test.size(); ++i) {
      test[i]->SetColor(glm::vec3(0, 0, 1));
    }

    break;
  }

  glutPostRedisplay();
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
  glutInitWindowPosition(50, 50);
  win_id = glutCreateWindow("Quadtree demo");
  
  glutDisplayFunc(display);
  glutIdleFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse_click);
  glutKeyboardFunc(key_press);
  glutSpecialFunc(key_press_special);

  initGL();
  glutMainLoop();
  return 0;
}