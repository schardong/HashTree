#include <iostream>
#include "quadtree.h"

using std::cout;
using std::endl;

void TestAABB();
void TestPointInAABB();
void TestQuadNodeSplit();
void TestAddPoint();
void TestQuadTree();

int main()
{
//  TestAABB();
//  TestPointInAABB();
//  TestQuadNodeSplit();
//  TestAddPoint();
  TestQuadTree();
  return 0;
}

void TestAABB()
{
  cout << "Test AABB.\n";
  AABB bbox1;
  AABB bbox2;

  cout << bbox1.Intersect(bbox2) << endl;
  bbox2.bl_corner = glm::vec2(1, 0);
  cout << bbox1.Intersect(bbox2) << endl;
  bbox2.bl_corner = glm::vec2(2, 0);
  cout << bbox1.Intersect(bbox2) << endl;
  bbox2.bl_corner = glm::vec2(-1, 0);
  cout << bbox1.Intersect(bbox2) << endl;
  bbox2.bl_corner = glm::vec2(-2, 0);
  cout << bbox1.Intersect(bbox2) << endl;
  bbox2.edge_sz = 3;
  cout << bbox1.Intersect(bbox2) << endl;
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
  cout << "TEST QUADTREE.\n";
  QuadTree* qtree = new QuadTree();

  for(int i = 0; i < 128; i++) {
    qtree->AddPoint(glm::vec2((double) i / 128.f + 0.001, 0.3));
  }

  delete qtree;
}
