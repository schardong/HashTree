#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glm/glm.hpp>

class QuadTree;
class QuadTreeNode;
class BBox;

class Mesh
{
public:
  Mesh(QuadTree* qt);
  ~Mesh();

  void SetBaseMesh(std::vector<glm::vec2*> domain);
  void Triangulate();
  void draw();

private:
  QuadTree* m_tree;
  glm::vec3 m_color;
  std::vector<BBox*> m_faces;
  std::vector<glm::vec2*> m_vertices;
};

#endif  // MESH_H
