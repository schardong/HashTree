#ifndef EDGE_H
#define EDGE_H

class Edge
{
public:
  vec2* u;
  vec2* v;

  Edge(vec2* u1, vec2* v1) :
    u(u1), v(v1)
  {}
};

#endif // EDGE_H