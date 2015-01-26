#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>
#include <glm/glm.hpp>

#include "quadtreenode.h"

class QuadTree
{
public:
  QuadTree(BBox *bbox, BBOX_TYPE t = AXIS_ALIGNED, size_t num_points_node = 64);
  virtual ~QuadTree();

  virtual bool AddPoint(glm::vec2 p);
  virtual std::vector<glm::vec2> GetPointsInRange(BBox* range);

  size_t GetMaxPointsNode()
  {
    return max_points_node;
  }

  size_t GetNumPoints()
  {
    return m_num_points;
  }

  int GetDepth()
  {
    return depth;
  }

private:
  size_t max_points_node;
  size_t m_num_points;
  int depth;
  QuadTreeNode* root_node;
};

#endif // QUADTREE_H
