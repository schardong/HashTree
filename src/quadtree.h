#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>
#include <glm/glm.hpp>

#include "quadtreenode.h"
#include "edge.h"

class QuadTree
{
public:
  QuadTree(BBox *bbox, size_t num_points_node = 64);
  virtual ~QuadTree();

  virtual bool AddPoint(Vertex* p);
  virtual std::vector<Vertex*> GetPointsInRange(BBox* range);

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

  QuadTreeNode* GetRoot()
  {
    return root_node;
  }

  std::vector<QuadTreeNode*> GetLeaves();
  void draw();
  void delEmptyLeaves(); //tmp method

private:
  size_t max_points_node;
  size_t m_num_points;
  int depth;
  QuadTreeNode* root_node;
};

void balance_tree(QuadTree*);
void enforce_corners(QuadTree*);
void delete_out_nodes(QuadTree*, std::vector<Vertex *> hull_vertices);

#endif // QUADTREE_H
