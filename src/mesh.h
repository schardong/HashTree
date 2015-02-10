#ifndef MESH_H
#define MESH_H

#include <vector>
#include "edge.h"

class QuadTree;
class QuadTreeNode;
class BBox;

class Mesh
{
public:
  Mesh(QuadTree* qt);
  ~Mesh();

  void SetBaseMesh(std::vector<Vertex*> domain);
  void Triangulate();
  void draw();

private:
  QuadTree* m_tree;
  glm::vec3 m_color;
  std::vector<BBox*> m_faces;
  std::vector<Vertex*> m_vertices;
  std::vector<Edge*> m_edges;
};

#endif  // MESH_H
