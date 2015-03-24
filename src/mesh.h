#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glm/glm.hpp>

#include "vertex.h"

class QuadTree;
class QuadTreeNode;
class BBox;

class Mesh
{
public:
  Mesh(QuadTree* qt);
  ~Mesh();

  void draw();


protected:
  int next_edge(int curr_edge)
  {
    return 4 * std::ceil(curr_edge / 4) + (curr_edge + 1) % 4;
  }

  int prev_edge(int curr_edge)
  {
    return 4 * std::ceil(curr_edge / 4) + (curr_edge + 3) % 4;
  }

private:
  QuadTree* m_tree;
  glm::vec3 m_color;

  std::vector<vertex> m_vertices;
  std::vector<int> m_edges;  
};

#endif  // MESH_H
