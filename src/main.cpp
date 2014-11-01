#include <iostream>
#include "quad.h"
#include "quadtreenode.h"

using std::cout;
using std::endl;

void PrintQuad(Quad* q)
{
  cout << *q->x1() << ", " << *q->y1() << ", " << *q->z1() << endl;
  cout << *q->x2() << ", " << *q->y2() << ", " << *q->z2() << endl;
  cout << *q->x_mid() << ", " << *q->y_mid() << ", " << *q->z_mid() << endl;
}

void TestOldQuad()
{
  Quad* q = new Quad;

  q->split();

  PrintQuad(q);
  cout << endl;
  PrintQuad(q->quads[0][0]);
  cout << endl;
  PrintQuad(q->quads[0][1]);
  cout << endl;
  PrintQuad(q->quads[1][0]);
  cout << endl;
  PrintQuad(q->quads[1][1]);
  cout << endl;

  delete q;
}

void TestAABB()
{
  AABB bbox1;
  AABB bbox2;

  cout << bbox1.Intersect(bbox2) << endl;
  bbox2.center = glm::vec2(1, 0);
  cout << bbox1.Intersect(bbox2) << endl;
  bbox2.center = glm::vec2(2, 0);
  cout << bbox1.Intersect(bbox2) << endl;
  bbox2.center = glm::vec2(3, 9);
  cout << bbox1.Intersect(bbox2) << endl;
}

int main()
{
  //TestOldQuad();
  TestAABB();
  return 0;
}
