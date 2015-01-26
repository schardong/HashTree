#include <iostream>
#include <glm/glm.hpp>
#include <array>
#include "quadtree.h"

using std::cout;
using std::endl;
using std::array;

void TestRhombus();
void TestPointInRhombus();
void TestQuadNodeRSplit();
void TestGetPointsRhombusTree();
void TestQuadTreeRhombus();

int main()
{
//  TestRhombus();
//  TestPointInRhombus();
//  TestQuadNodeRSplit();
//  TestGetPointsRhombusTree();
  TestQuadTreeRhombus();
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
  QuadTree* qt = new QuadTree(r0, RHOMBUS, 4);

  const int MAX_I = 4;
  const int MAX_J = 4;

  for(int i = 0; i < MAX_I; ++i) {
    for(int j = 0; j < MAX_J; ++j) {
      vec2 v = vec2((2.f * i) / (float) MAX_I, (2.f * j) / (float) MAX_J);
      qt->AddPoint(v);
    }
  }

  cout << qt->GetNumPoints() << "/" << MAX_I * MAX_J << " points added.\n";

  delete qt;
}

//void TestQuadTree()
//{
//  cout << "Test Quadtree.\n";
//  QuadTree* qtree = new QuadTree(64);
//
//  for(int i = 0; i < 1024; ++i) {
//    for(int j = 0; j < 1024; ++j) {
//      qtree->AddPoint(glm::vec2(i / 1024.f, j / 1024.f));
//    }
//  }
//
//  cout << "  Tree depth = " << qtree->GetDepth() << endl;
//  cout << "  Number of points = " << qtree->GetNumPoints() << endl;
//  delete qtree;
//}

//void TestGetPointsTree()
//{
//  using std::vector;
//  using glm::vec2;
//
//  cout << "Test GetPointsTree.\n";
//  QuadTree* qtree = new QuadTree(4);
//
//  for(int i = 0; i < 4; ++i) {
//    for(int j = 0; j < 4; ++j) {
//      qtree->AddPoint(vec2(i / 4.f, j / 4.f));
//    }
//  }
//
//  vector<vec2> points = qtree->GetPointsInRange(new AABB(vec2(0.0, 0.0), 0.51));
//
//  cout << "  Found " << points.size() << " points:\n";
//  for(auto it = points.begin(); it != points.end(); ++it)
//    cout << "    p = (" << it->x << ", " << it->y << ")" << endl;
//
//  cout << "  Tree depth = " << qtree->GetDepth() << endl;
//  cout << "  Number of points = " << qtree->GetNumPoints() << endl;
//  delete qtree;
//}
