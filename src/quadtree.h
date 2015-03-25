#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>
#include <glm/glm.hpp>

class vertex;
class QuadTreeNode;
class BBox;

class QuadTree
{
public:
  QuadTree(BBox* bbox,
    size_t num_points_node = 64,
    int max_depth = -1,
    std::vector<vertex*> p = std::vector<vertex*>());

  virtual ~QuadTree();

  virtual bool AddPoint(vertex* p);
  virtual std::vector<vertex*> GetPointsInRange(BBox* range);
  std::vector<QuadTreeNode*> GetLeaves(int level = -1);
  std::vector<QuadTreeNode*> GetUnconformingLeaves(int level);
  std::vector<QuadTreeNode*> GetAllNodes();
  void Draw();

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

private:
  size_t m_max_points_node;
  size_t m_num_points;
  int m_max_depth;
  int m_depth;
  QuadTreeNode* m_root_node;

  std::vector<QuadTreeNode*> get_all_leaves();
  std::vector<QuadTreeNode*> get_leaves(int level);
};

bool pnpoly(vertex, std::vector<vertex>);

void balance_tree(QuadTree*);

std::vector<QuadTreeNode*> get_populated_leaves(QuadTree* qt, int level = -1);

std::vector<QuadTreeNode*> get_first_nbrs(QuadTreeNode* node,
                                          std::vector<QuadTreeNode*> leaves);

std::vector<QuadTreeNode*> get_nbrs_vertex(QuadTreeNode* node,
                                           std::vector<QuadTreeNode*> leaves,
                                           int vertex_idx);

#endif // QUADTREE_H
