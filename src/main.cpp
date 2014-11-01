#include <iostream>
#include "quad.h"

using std::cout;
using std::endl;

void PrintQuad(Quad* q)
{
  cout << *q->x1() << ", " << *q->y1() << ", " << *q->z1() << endl;
  cout << *q->x2() << ", " << *q->y2() << ", " << *q->z2() << endl;
  cout << *q->x_mid() << ", " << *q->y_mid() << ", " << *q->z_mid() << endl;
}

int main()
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
  return 0;
}
