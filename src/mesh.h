#ifndef MESH_H
#define MESH_H

#include <vector>
#include "edge.h"

class QuadTree;
class QuadTreeNode;

class Mesh
{
  using namespace std;
public:

private:
  vector<Vertex*> m_vertices;
};

#endif  // MESH_H
