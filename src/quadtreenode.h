#ifndef QUADTREENODE_H
#define QUADTREENODE_H

#include <vector>
#include <glm/glm.hpp>

struct AABB
{
  glm::vec2 center;
  size_t edge_sz;

  AABB(glm::vec2 c = glm::vec2(0, 0), size_t edge = 1) :
    center(c), edge_sz(edge)
  {}

  bool Intersect(AABB& rhs)
  {
    glm::vec2 c = center - rhs.center;
    if(glm::dot(c, c) <= pow(edge_sz + rhs.edge_sz, 2)) return true;
    return false;
  }
};

class QuadTreeNode
{
public:
  QuadTreeNode(AABB box = AABB());
  virtual ~QuadTreeNode();

  virtual bool IsLeaf()
  {
    return (!children[0] && !children[1] && !children[2] && !children[3]);
  }

  virtual void Split() {}

private:
  AABB bbox;
  QuadTreeNode* children[4];
  glm::vec2* points;
};

#endif // QUADTREENODE_H
