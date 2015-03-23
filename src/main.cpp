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
#include "quadtreenode.h"
#include "mesh.h"
#include "vertex.h"
#include "bbox.h"
#include "CImg.h"

using namespace std;
using namespace glm;
using namespace cimg_library;

static int WIN_WIDTH = 512;
static int WIN_HEIGHT = 512;
static int win_id = -1;

QuadTree* qt = nullptr;
Mesh* g_mesh = nullptr;

vector<vertex*> g_vertices;

#define LEFT 0.0001f
#define RIGHT 1.f
#define TOP 1.f
#define BOTTOM 0.0001f

void createTree()
{
  array<vertex, 4> v0 = { vertex(vec2(LEFT, BOTTOM)),
                          vertex(vec2(RIGHT, BOTTOM)),
                          vertex(vec2(RIGHT, TOP)),
                          vertex(vec2(LEFT, TOP)) };
	BBox* r0 = new BBox(v0);

	//AXELLE
	CImg<unsigned char> image;
	image.load_bmp("faille4.bmp");
	int w = image.width();
	int h = image.height();
	for (int i = 0; i < w; i++){
		for (int j = 0; j < h; j++){
			if (image(i, j) == 0){ //black g_vertices
				float normi = (float)i / w;
				float normj = (float)j / h;
        vertex* v = new vertex(vec2(normi, 1-normj));
        if (r0->PointInBox(v->GetCoord()))
					g_vertices.push_back(v);
				else
					delete v;
			}
		}
	}

  qt = new QuadTree(r0, 10, 4);//, -1, g_vertices);
  for(size_t i = 0; i < g_vertices.size(); ++i)
    qt->AddPoint(g_vertices[i]);

}

void createTestTree()
{
  array<vertex, 4> v0 = { vertex(vec2(LEFT, BOTTOM)),
                          vertex(vec2(RIGHT, BOTTOM)),
                          vertex(vec2(RIGHT, TOP)),
                          vertex(vec2(LEFT, TOP)) };
	BBox* r0 = new BBox(v0);

  qt = new QuadTree(r0, 10, 4);//, -1, g_vertices);
  qt->GetRoot()->Split();
  qt->GetRoot()->GetChild(0)->Split();
  //qt->GetRoot()->GetChild(1)->Split();
  qt->GetRoot()->GetChild(0)->GetChild(3)->Split();

  
  map<int, int> shared_verts;
  vector<QuadTreeNode*> leaves = qt->GetLeaves();
  
  for(int i = 0; i < leaves.size(); ++i) {
    BBox* box = leaves[i]->GetBBox();

    for(int j = 0; j < 4; ++j) {
      int vid = box->GetCorner(j).GetId();
      if(shared_verts.find(vid) == shared_verts.end())
        shared_verts[vid] = 1;
      else
        shared_verts[vid]++;
    }
  }

  printf("number of vertices: %d\n", shared_verts.size());
}

void initGL()
{
  //createTree();
  createTestTree();
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
  for(size_t i = 0; i < g_vertices.size(); ++i)
    glVertex2f(g_vertices[i]->GetCoord().x, g_vertices[i]->GetCoord().y);
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
      vertex* v = new vertex(vec2(x / (float)WIN_WIDTH, 1.f - y / (float)WIN_HEIGHT));
      if(qt->AddPoint(v))
        g_vertices.push_back(v);
      else
        delete v;
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

    for(size_t i = 0; i < g_vertices.size(); ++i) {
      memset(g_vertices[i], 0, sizeof(vec2));
      delete g_vertices[i];
      g_vertices[i] = nullptr;
    }

    g_vertices.clear();

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
    QuadTreeNode* node = qt->GetRoot()->GetChild(SW)->GetChild(SW);
    //If you want all the neighbors of any level use this call.
    //vector<QuadTreeNode*> test = get_nbrs_vertex(node, qt->GetAllNodes(), 2);
    //Now, if you want only leaf nodes, use this call.
    vector<QuadTreeNode*> test = get_nbrs_vertex(node, qt->GetLeaves(), 2);
    for(size_t i = 0; i < test.size(); ++i) {
      test[i]->SetColor(glm::vec3(0, 0, 1));
    }

    cout << "Number of nodes that share this vertex: " << test.size() << endl;
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
