#include "quadtree.h"
#include "quadtreenode.h"

QuadTree::QuadTree(BBox* bbox, BBOX_TYPE t, size_t num_points_node) :
  m_num_points(0),
  depth(0),
  max_points_node(num_points_node)
{
  root_node = new QuadTreeNode(bbox, t, num_points_node);
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
  ++m_num_points;
}

std::vector<glm::vec2> QuadTree::GetPointsInRange(BBox *range)
{
  return root_node->GetPointsInRange(range);
}
