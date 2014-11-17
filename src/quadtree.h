#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>
#include <glm/glm.hpp>

#include "quadtreenode.h"

class QuadTree
{
public:
  QuadTree(size_t num_points = 64, AABB* bbox = new AABB);
  virtual ~QuadTree();

  virtual void AddPoint(glm::vec2 p);
  virtual std::vector<glm::vec2> GetPointsInRange(AABB* range);

  size_t GetMaxPointsNode()
  {
    return max_points_node;
  }

  size_t GetNumPoints()
  {
    return num_points;
  }

  int GetDepth()
  {
    return depth;
  }

private:
  size_t max_points_node;
  size_t num_points;
  int depth;
  QuadTreeNode* root_node;
};

#endif // QUADTREE_H
