#include <iostream>
#include "quad.h"

int main()
{
  using std::cout;
  using std::endl;

  Quad* q = new Quad;

  cout << *q->x1() << ", " << *q->y1() << ", " << *q->z1() << endl;
  cout << *q->x2() << ", " << *q->y2() << ", " << *q->z2() << endl;
  cout << *q->x_mid() << ", " << *q->y_mid() << ", " << *q->z_mid() << endl;

  q->split();

  delete q;
  return 0;
}
