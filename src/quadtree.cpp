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

bool QuadTree::AddPoint(glm::vec2 p)
{
  int r_depth = root_node->AddPoint(p);
  if(r_depth == -1)
    return false;
  depth = r_depth;
  ++m_num_points;
  return true;
}

std::vector<glm::vec2> QuadTree::GetPointsInRange(BBox *range)
{
  return root_node->GetPointsInRange(range);
}

void QuadTree::BalanceTree()
{
  root_node->BalanceTree();
}

void QuadTree::EnforceCornerCond()
{
  root_node->EnforceCornerCond();
}

void QuadTree::draw()  
{
  root_node->draw();
}

void QuadTree::delEmptyLeaves()
{
  root_node->delEmptyLeaves();
}
