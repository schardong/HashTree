#ifndef QUADTREENODE_H
#define QUADTREENODE_H

#include <vector>
#include <glm/glm.hpp>

struct AABB
{
  glm::vec2 center;
  glm::vec2 h_dim;

  AABB(glm::vec2 c = glm::vec2(0, 0), glm::vec2 dim = glm::vec2(0, 0)) :
    center(c), h_dim(dim)
  {}

  bool Intersect(AABB& rhs)
  {
    return false;
  }
};

class QuadTreeNode
{
public:
  QuadTreeNode(AABB box = AABB());
  virtual ~QuadTreeNode();

  bool IsLeaf()
  {
    return (!children[0] && !children[1] && !children[2] && !children[3]);
  }

  void Split() {}

private:
  AABB bbox;
  QuadTreeNode* children[4];
  glm::vec2* points;
};

#endif // QUADTREENODE_H
