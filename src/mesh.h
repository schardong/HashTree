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
  Mesh(QuadTree* qt = nullptr);
  ~Mesh();

  void BuildGeometry(QuadTree*);
  void Draw();

  glm::vec3 GetColor()
  {
    return m_color;
  }

  void SetColor(glm::vec3 c)
  {
    m_color = c;
  }

protected:
  int next_edge(int curr_edge)
  {
    return static_cast<int>(4 * std::floor(curr_edge / 4) + (curr_edge + 1) % 4);
  }

  int prev_edge(int curr_edge)
  {
    return static_cast<int>(4 * std::floor(curr_edge / 4) + (curr_edge + 3) % 4);
  }

  virtual void cleanup()
  {
    m_vertices.clear();
    m_edges.clear();
    m_opposites.clear();
  }

private:
  glm::vec3 m_color;

  std::vector<vertex> m_vertices;
  std::vector<int> m_edges; 
  std::vector<int> m_opposites;

  void build_geom_container(std::vector<QuadTreeNode*>);
  void build_he_container(std::vector<QuadTreeNode*>);
  void build_opp_container();
};

#endif  // MESH_H
