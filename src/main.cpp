#include <iostream>
#include <glm/glm.hpp>
#include "quadtree.h"

using std::cout;
using std::endl;

void TestAABB();
void TestPointInAABB();
void TestQuadNodeSplit();
void TestAddPoint();
void TestQuadTree();
void TestGetPointsTree();
void TestRhombus();

int main()
{
  //TestAABB();
  TestRhombus();
//  TestPointInAABB();
//  TestQuadNodeSplit();
//  TestAddPoint();
//  TestQuadTree();
  //TestGetPointsTree();
  return 0;
}

void TestAABB()
{
  cout << "Test AABB.\n";
  AABB bbox1;
  AABB bbox2;

  cout << bbox1.Intersect(bbox2) << endl;

  bbox2.SetCorner(glm::vec2(1, 0));
  cout << bbox1.Intersect(bbox2) << endl;

  bbox2.SetCorner(glm::vec2(2, 0));
  cout << bbox1.Intersect(bbox2) << endl;

  bbox2.SetCorner(glm::vec2(-1, 0));
  cout << bbox1.Intersect(bbox2) << endl;

  bbox2.SetCorner(glm::vec2(-2, 0));
  cout << bbox1.Intersect(bbox2) << endl;

  bbox2.SetEdgeSz(3);
  cout << bbox1.Intersect(bbox2) << endl;

  bbox2.GetCorner() = glm::vec2(-1, -1);
  bbox2.SetEdgeSz(2);
  cout << bbox1.Intersect(bbox2) << endl;

  bbox2.GetCorner() = glm::vec2(0, 0);
  bbox2.SetEdgeSz(0.25);
  cout << bbox2.Intersect(bbox1) << endl;
}

void TestPointInAABB()
{
  cout << "Test point in AABB.\n";
  AABB bbox1;

  glm::vec2 p(0.5, 0.5);
  cout << bbox1.PointInBox(p) << endl;

  p = glm::vec2(0, 0);
  cout << bbox1.PointInBox(p) << endl;

  p = glm::vec2(1, 0);
  cout << bbox1.PointInBox(p) << endl;

  p = glm::vec2(0, 1);
  cout << bbox1.PointInBox(p) << endl;

  p = glm::vec2(1, 1);
  cout << bbox1.PointInBox(p) << endl;

  p = glm::vec2(0.2, -0.2);
  cout << bbox1.PointInBox(p) << endl;

  p = glm::vec2(-0.2, -0.2);
  cout << bbox1.PointInBox(p) << endl;

  p = glm::vec2(0.2, 0.2);
  cout << bbox1.PointInBox(p) << endl;
}

void TestQuadNodeSplit()
{
  cout << "Test Quad node split.\n";

  using glm::vec2;
  QuadTreeNode* qnode = new QuadTreeNode(4);

  qnode->AddPoint(vec2(0.25, 0.25));
  qnode->AddPoint(vec2(0.25, 0.35));
  qnode->AddPoint(vec2(0.25, 0.45));

  qnode->AddPoint(vec2(0.75, 0.25));
  qnode->AddPoint(vec2(0.93, 0.45));
  qnode->AddPoint(vec2(0.63, 0.00));
  qnode->AddPoint(vec2(0.61, 0.05));

  qnode->AddPoint(vec2(0.75, 0.75));

  qnode->AddPoint(vec2(0.25, 0.75));

  qnode->Split();

  delete qnode;
}

void TestAddPoint()
{
  cout << "Test QuadNode add point.\n";
  using glm::vec2;
  QuadTreeNode* qnode = new QuadTreeNode(4);

  qnode->AddPoint(vec2(0.25, 0.25));
  qnode->AddPoint(vec2(0.75, 0.25));
  qnode->AddPoint(vec2(0.93, 0.95));
  qnode->AddPoint(vec2(0.03, 0.70));
  qnode->AddPoint(vec2(0.13, 0.20));

  delete qnode;
}

void TestQuadTree()
{
  cout << "Test Quadtree.\n";
  QuadTree* qtree = new QuadTree(64);

  for(int i = 0; i < 1024; ++i) {
    for(int j = 0; j < 1024; ++j) {
      qtree->AddPoint(glm::vec2(i / 1024.f, j / 1024.f));
    }
  }

  cout << "  Tree depth = " << qtree->GetDepth() << endl;
  cout << "  Number of points = " << qtree->GetNumPoints() << endl;
  delete qtree;
}

void TestGetPointsTree()
{
  using std::vector;
  using glm::vec2;

  cout << "Test GetPointsTree.\n";
  QuadTree* qtree = new QuadTree(4);

  for(int i = 0; i < 4; ++i) {
    for(int j = 0; j < 4; ++j) {
      qtree->AddPoint(vec2(i / 4.f, j / 4.f));
    }
  }

  vector<vec2> points = qtree->GetPointsInRange(new AABB(vec2(0.0, 0.0), 0.51));

  cout << "  Found " << points.size() << " points:\n";
  for(auto it = points.begin(); it != points.end(); ++it)
    cout << "    p = (" << it->x << ", " << it->y << ")" << endl;

  cout << "  Tree depth = " << qtree->GetDepth() << endl;
  cout << "  Number of points = " << qtree->GetNumPoints() << endl;
  delete qtree;
}

void TestRhombus()
{
  Rhombus* r = new Rhombus;

  delete r;
}