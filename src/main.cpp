#include <iostream>
#include <glm/glm.hpp>
#include <array>

#include "quadtree.h"

#include <GL/glut.h>
 
using std::cout;
using std::endl;
using std::array;
using glm::vec2;

array<vec2, 4> v0 = {vec2(0, 0), vec2(1, 0), vec2(2, 1), vec2(1, 1)};
Rhombus* r0;

void initGL()
{
  r0 = new Rhombus(v0);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
 
void display()
{
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(1.f, 0.f, 0.f);

  glBegin(GL_QUADS);
  for(int i = 0; i < 4; ++i)
    glVertex2f(v0[i].x, v0[i].y);
  glEnd();

  glFlush();
}

void reshape(GLsizei width, GLsizei height)
{
  if(height == 0)
    height = 1;
  GLfloat aspect = (GLfloat)width / (GLfloat)height;
 
  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if(width >= height) {
    gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
  } else {
    gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
  }
}
 
int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitWindowSize(640, 480);
  glutInitWindowPosition(50, 50);
  glutCreateWindow("Viewport Transform");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  initGL();
  glutMainLoop();
  return 0;
}

void TestRhombus()
{
  using namespace glm;
  array<vec2, 4> v1 = {vec2(0, 0), vec2(1, 0), vec2(1, 1), vec2(0, 1)};
  array<vec2, 4> v2 = {vec2(3.5, 0.3), vec2(4, 0.3), vec2(4.5, 1.5), vec2(4.5, 1.5)};
  Rhombus r1(v1);
  Rhombus r2(v2);

  cout << "Test Rhombus box.\n";

  cout << r1.Intersect(r2) << endl;
  cout << r2.Intersect(r1) << endl;
}

void TestPointInRhombus()
{
  using namespace glm;
  cout << "Test Point in Rhombus box.\n";

  array<vec2, 4> v1 = {vec2(0, 0), vec2(1, 0), vec2(2, 1), vec2(1, 1)};
  Rhombus r1(v1);

  vec2 p = vec2(0.5, 0.3);
  cout << r1.PointInBox(p) << endl;

  p = vec2(-0.1, 0.1);
  cout << r1.PointInBox(p) << endl;

  p = vec2(0, 0);
  cout << r1.PointInBox(p) << endl;

  p = vec2(0, 1);
  cout << r1.PointInBox(p) << endl;

  p = vec2(1, 0);
  cout << r1.PointInBox(p) << endl;

  p = vec2(1, 1);
  cout << r1.PointInBox(p) << endl;

  p = vec2(0.5, 0);
  cout << r1.PointInBox(p) << endl;

  p = vec2(0, 0.5);
  cout << r1.PointInBox(p) << endl;
}

void TestQuadNodeRSplit()
{
  cout << "Test QuadNode Rhombus split.\n";
  using glm::vec2;
  array<vec2, 4> v1 = {vec2(0, 0), vec2(1, 0), vec2(2, 1), vec2(1, 1)};
  Rhombus* r1 = new Rhombus(v1);
  QuadTreeNode* qnode = new QuadTreeNode(r1, RHOMBUS, 4);

  cout << "Number of points in the node: " << qnode->GetNumPoints() << "\tDepth: " << qnode->GetDepth() << endl;
  qnode->AddPoint(vec2(0.25, 0.25));
  cout << "Number of points in the node: " << qnode->GetNumPoints() << "\tDepth: " << qnode->GetDepth() << endl;
  qnode->AddPoint(vec2(0.75, 0.25));
  cout << "Number of points in the node: " << qnode->GetNumPoints() << "\tDepth: " << qnode->GetDepth() << endl;
  qnode->AddPoint(vec2(0.93, 0.95));
  cout << "Number of points in the node: " << qnode->GetNumPoints() << "\tDepth: " << qnode->GetDepth() << endl;
  qnode->AddPoint(vec2(0.03, 0.70));
  cout << "Number of points in the node: " << qnode->GetNumPoints() << "\tDepth: " << qnode->GetDepth() << endl;
  qnode->AddPoint(vec2(0.13, 0.20));
  cout << "Number of points in the node: " << qnode->GetNumPoints() << "\tDepth: " << qnode->GetDepth() << endl;

  qnode->AddPoint(vec2(1.13, 0.20));
  cout << "Number of points in the node: " << qnode->GetNumPoints() << "\tDepth: " << qnode->GetDepth() << endl;

  qnode->AddPoint(vec2(1.2, 0.90));
  cout << "Number of points in the node: " << qnode->GetNumPoints() << "\tDepth: " << qnode->GetDepth() << endl;

  qnode->AddPoint(vec2(1.5, 0.70));
  cout << "Number of points in the node: " << qnode->GetNumPoints() << "\tDepth: " << qnode->GetDepth() << endl;

  qnode->AddPoint(vec2(1.0, 0.20));
  cout << "Number of points in the node: " << qnode->GetNumPoints() << "\tDepth: " << qnode->GetDepth() << endl;

  qnode->AddPoint(vec2(1.5, 0.80));
  cout << "Number of points in the node: " << qnode->GetNumPoints() << "\tDepth: " << qnode->GetDepth() << endl;

  delete qnode;
}

void TestGetPointsRhombusTree()
{
  cout << "Test get points in range.\n";
  using glm::vec2;
  using std::vector;
  array<vec2, 4> v1 = {vec2(0, 0), vec2(1, 0), vec2(2, 1), vec2(1, 1)};
  Rhombus* r1 = new Rhombus(v1);
  QuadTreeNode* qnode = new QuadTreeNode(r1, RHOMBUS, 4);

  cout << "Number of points in the node: " << qnode->GetNumPoints() << "\tDepth: " << qnode->GetDepth() << endl;
  qnode->AddPoint(vec2(0.25, 0.25));
  cout << "Number of points in the node: " << qnode->GetNumPoints() << "\tDepth: " << qnode->GetDepth() << endl;
  qnode->AddPoint(vec2(0.75, 0.25));
  cout << "Number of points in the node: " << qnode->GetNumPoints() << "\tDepth: " << qnode->GetDepth() << endl;
  qnode->AddPoint(vec2(0.93, 0.95));
  cout << "Number of points in the node: " << qnode->GetNumPoints() << "\tDepth: " << qnode->GetDepth() << endl;
  qnode->AddPoint(vec2(0.03, 0.70));
  cout << "Number of points in the node: " << qnode->GetNumPoints() << "\tDepth: " << qnode->GetDepth() << endl;
  qnode->AddPoint(vec2(0.13, 0.20));
  cout << "Number of points in the node: " << qnode->GetNumPoints() << "\tDepth: " << qnode->GetDepth() << endl;

  qnode->AddPoint(vec2(1.13, 0.20));
  cout << "Number of points in the node: " << qnode->GetNumPoints() << "\tDepth: " << qnode->GetDepth() << endl;

  qnode->AddPoint(vec2(1.2, 0.90));
  cout << "Number of points in the node: " << qnode->GetNumPoints() << "\tDepth: " << qnode->GetDepth() << endl;

  qnode->AddPoint(vec2(1.5, 0.70));
  cout << "Number of points in the node: " << qnode->GetNumPoints() << "\tDepth: " << qnode->GetDepth() << endl;

  qnode->AddPoint(vec2(1.0, 0.20));
  cout << "Number of points in the node: " << qnode->GetNumPoints() << "\tDepth: " << qnode->GetDepth() << endl;

  qnode->AddPoint(vec2(1.5, 0.80));
  cout << "Number of points in the node: " << qnode->GetNumPoints() << "\tDepth: " << qnode->GetDepth() << endl;

  vector<vec2> p = qnode->GetPointsInRange(r1);
  for(auto it = p.begin(); it != p.end(); ++it) {
    cout << it->x << " " << it->y << endl;
  }

  delete qnode;
}

void TestQuadTreeRhombus()
{
  using namespace glm;
  using namespace std;

  cout << "Test rhombus quadtree structure.\n";
  array<vec2, 4> c0 = {vec2(0, 0), vec2(1, 0), vec2(2, 1), vec2(1, 1)};
  Rhombus* r0 = new Rhombus(c0);
  QuadTree* qt = new QuadTree(r0, RHOMBUS);

  const int MAX_I = 1024;
  const int MAX_J = 1024;

  for(int i = 0; i < MAX_I; ++i) {
    for(int j = 0; j < MAX_J; ++j) {
      vec2 v = vec2((2.f * i) / (float) MAX_I, (2.f * j) / (float) MAX_J);
      qt->AddPoint(v);
    }
  }

  cout << qt->GetNumPoints() << "/" << MAX_I * MAX_J << " points added.\n";

  delete qt;
}
