#ifndef QUADTREE_H
#define QUADTREE_H

#include <glm/glm.hpp>
#include "quadtreenode.h"

class QuadTree
{
public:
  QuadTree(size_t num_points = 64, AABB* bbox = new AABB);
  virtual ~QuadTree();

  virtual void AddPoint(glm::vec2 p);

  size_t GetMaxPoints()
  {
    return max_points_node;
  }

private:
  size_t max_points_node;
  size_t num_points;
  QuadTreeNode* root_node;
};

#endif // QUADTREE_H
