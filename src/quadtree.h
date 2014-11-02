#ifndef QUADTREE_H
#define QUADTREE_H

#include <glm/glm.hpp>

class QuadTreeNode;

class QuadTree
{
public:
  QuadTree();
  virtual ~QuadTree();

  virtual void AddPoint(glm::vec2 p);

private:
  size_t max_points;
  QuadTreeNode* root_node;
};

#endif // QUADTREE_H