#ifndef EDGE_H
#define EDGE_H

#include <vector>
#include <glm/glm.hpp>

class Edge;

class Vertex
{
public:
  int id;
  glm::vec2 p;
  std::vector<Edge*> edges;

  Vertex(glm::vec2 p1 = glm::vec2(0)) :
    p(p1)
  {}
};

class Edge
{
public:
  Vertex* u;
  Vertex* v;

  Edge(Vertex* u1 = nullptr, Vertex* v1 = nullptr) :
    u(u1), v(v1)
  {}
};

#endif // EDGE_H