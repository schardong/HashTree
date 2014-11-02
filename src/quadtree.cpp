#include "quadtree.h"
#include "quadtreenode.h"

QuadTree::QuadTree(size_t num_points, AABB* bbox) : num_points(0)
{
  root_node = new QuadTreeNode(num_points, bbox);
  max_points_node = num_points;
  depth = 0;
}

QuadTree::~QuadTree()
{
  if(root_node)
    delete root_node;
  root_node = nullptr;
}

void QuadTree::AddPoint(glm::vec2 p)
{
  depth = root_node->AddPoint(p);
  ++num_points;
}
