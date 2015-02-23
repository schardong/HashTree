#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>
#include <glm/glm.hpp>

#include "quadtreenode.h"
#include "edge.h"

class QuadTree
{
public:
  QuadTree(BBox *bbox,
    size_t num_points_node = 64,
    int max_depth = -1,
    std::vector<Vertex*> p = std::vector<Vertex*>());

  virtual ~QuadTree();

  virtual bool AddPoint(Vertex* p);
  virtual std::vector<Vertex*> GetPointsInRange(BBox* range);

  size_t GetMaxPointsNode()
  {
    return m_max_points_node;
  }

  int GetMaxDepth()
  {
    return m_max_depth;
  }

  size_t GetNumPoints()
  {
    return m_num_points;
  }

  int GetDepth()
  {
    return m_depth;
  }

  QuadTreeNode* GetRoot()
  {
    return m_root_node;
  }

  std::vector<QuadTreeNode*> GetLeaves(int level = -1);
  std::vector<QuadTreeNode*> GetUnconformingLeaves(int level);
  void draw();

private:
  size_t m_max_points_node;
  size_t m_num_points;
  int m_max_depth;
  int m_depth;
  QuadTreeNode* m_root_node;

  std::vector<QuadTreeNode*> get_all_leaves();
  std::vector<QuadTreeNode*> get_leaves(int level);
};

bool pnpoly(glm::vec2, std::vector<glm::vec2>);
void balance_tree(QuadTree*);
std::vector<QuadTreeNode*> get_populated_leaves(QuadTree* qt, int level = -1);
std::vector<QuadTreeNode*> get_first_nbrs(QuadTreeNode* node,
                                          std::vector<QuadTreeNode*> leaves);

#endif // QUADTREE_H
