#include <iostream>
#include <glm/glm.hpp>
#include <array>
#include "quadtree.h"

using std::cout;
using std::endl;
using std::array;

void TestAABB();
void TestPointInAABB();
void TestQuadNodeSplit();
void TestAddPoint();
void TestQuadTree();
void TestGetPointsTree();

void TestRhombus();
void TestPointInRhombus();
void TestQuadNodeRSplit();

int main()
{
  TestRhombus();
  TestPointInRhombus();
  TestQuadNodeRSplit();
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

  delete qnode;
}

//void TestAABB()
//{
//  cout << "Test AABB.\n";
//  AABBox bbox1;
//  AABBox bbox2;
//
//  cout << bbox1.Intersect(bbox2) << endl;
//
//  bbox2.SetCorner(glm::vec2(1, 0));
//  cout << bbox1.Intersect(bbox2) << endl;
//
//  bbox2.SetCorner(glm::vec2(2, 0));
//  cout << bbox1.Intersect(bbox2) << endl;
//
//  bbox2.SetCorner(glm::vec2(-1, 0));
//  cout << bbox1.Intersect(bbox2) << endl;
//
//  bbox2.SetCorner(glm::vec2(-2, 0));
//  cout << bbox1.Intersect(bbox2) << endl;
//
//  bbox2.SetEdgeSz(3);
//  cout << bbox1.Intersect(bbox2) << endl;
//
//  bbox2.GetCorner() = glm::vec2(-1, -1);
//  bbox2.SetEdgeSz(2);
//  cout << bbox1.Intersect(bbox2) << endl;
//
//  bbox2.GetCorner() = glm::vec2(0, 0);
//  bbox2.SetEdgeSz(0.25);
//  cout << bbox2.Intersect(bbox1) << endl;
//}

//void TestPointInAABB()
//{
//  cout << "Test point in AABB.\n";
//  AABBox bbox1;
//
//  glm::vec2 p(0.5, 0.5);
//  cout << bbox1.PointInBox(p) << endl;
//
//  p = glm::vec2(0, 0);
//  cout << bbox1.PointInBox(p) << endl;
//
//  p = glm::vec2(1, 0);
//  cout << bbox1.PointInBox(p) << endl;
//
//  p = glm::vec2(0, 1);
//  cout << bbox1.PointInBox(p) << endl;
//
//  p = glm::vec2(1, 1);
//  cout << bbox1.PointInBox(p) << endl;
//
//  p = glm::vec2(0.2, -0.2);
//  cout << bbox1.PointInBox(p) << endl;
//
//  p = glm::vec2(-0.2, -0.2);
//  cout << bbox1.PointInBox(p) << endl;
//
//  p = glm::vec2(0.2, 0.2);
//  cout << bbox1.PointInBox(p) << endl;
//}

//void TestQuadNodeSplit()
//{
//  cout << "Test Quad node split.\n";
//
//  using glm::vec2;
//  QuadTreeNode* qnode = new QuadTreeNode(4);
//
//  qnode->AddPoint(vec2(0.25, 0.25));
//  qnode->AddPoint(vec2(0.25, 0.35));
//  qnode->AddPoint(vec2(0.25, 0.45));
//
//  qnode->AddPoint(vec2(0.75, 0.25));
//  qnode->AddPoint(vec2(0.93, 0.45));
//  qnode->AddPoint(vec2(0.63, 0.00));
//  qnode->AddPoint(vec2(0.61, 0.05));
//
//  qnode->AddPoint(vec2(0.75, 0.75));
//
//  qnode->AddPoint(vec2(0.25, 0.75));
//
//  qnode->Split();
//
//  delete qnode;
//}

//void TestAddPoint()
//{
//  cout << "Test QuadNode add point.\n";
//  using glm::vec2;
//  QuadTreeNode* qnode = new QuadTreeNode(4);
//
//  qnode->AddPoint(vec2(0.25, 0.25));
//  qnode->AddPoint(vec2(0.75, 0.25));
//  qnode->AddPoint(vec2(0.93, 0.95));
//  qnode->AddPoint(vec2(0.03, 0.70));
//  qnode->AddPoint(vec2(0.13, 0.20));
//
//  delete qnode;
//}

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
