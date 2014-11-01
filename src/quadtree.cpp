#include "quadtree.h"
#include "quadtreenode.h"

QuadTree::QuadTree()
{
  root_node = new QuadTreeNode(AABB(glm::vec2(0, 0), glm::vec2(1, 1)));
}

QuadTree::~QuadTree()
{
  if(root_node)
    delete root_node;
  root_node = nullptr;
}
