#ifndef QUADTREENODE_H
#define QUADTREENODE_H

#include "bbox.h"
#include <vector>
#include <queue>
#include <glm/glm.hpp>

enum BBOX_TYPE
{
  AXIS_ALIGNED,
  RHOMBUS
};

enum NODE_TYPE
{
  SW,
  SE,
  NW,
  NE,
  ROOT = 4
};

enum NBR_DIR
{
  S,
  E,
  N,
  W
};

class QuadTreeNode
{
public:
  QuadTreeNode();

  QuadTreeNode(BBox* box,
               BBOX_TYPE t = AXIS_ALIGNED,
               size_t max_npoints = 64,
               NODE_TYPE nt = ROOT,
               std::vector<glm::vec2> p = std::vector<glm::vec2>());

  virtual ~QuadTreeNode();

  virtual bool IsLeaf()
  {
    return (!children[0] && !children[1] && !children[2] && !children[3]);
  }

  virtual void Split();
  virtual int AddPoint(glm::vec2 p);
  virtual std::vector<glm::vec2> GetPointsInRange(BBox* range);

  size_t GetNumPoints()
  {
    return points.size();
  }

  size_t GetId()
  {
    return id;
  }

  BBox* GetBBox()
  {
    return bbox;
  }

  QuadTreeNode* GetChild(int idx)
  {
    assert(idx >= 0 && idx <= 3);
    return children[idx];
  }

  int GetDepth()
  {
    return depth;
  }

  void SetDepth(int d)
  {
    depth = d;
  }

  QuadTreeNode* GetParent()
  {
    return parent;
  }

  void SetParent(QuadTreeNode* p)
  {
    parent = p;
  }

  NODE_TYPE GetNodeType()
  {
    return node_type;
  }

  QuadTreeNode* FindNeighbor(NBR_DIR dir);
  void BalanceTree();
  void draw();
  void delEmptyLeaves();

private:
  size_t id;
  size_t max_points;
  int depth;
  BBox* bbox;
  BBOX_TYPE box_type;
  NODE_TYPE node_type;
  QuadTreeNode* children[4];
  QuadTreeNode* parent;
  std::vector<glm::vec2> points;

  void split_aabb();
  void split_rhombus();

  void draw_aabb();
  void draw_rhombus();

  QuadTreeNode* north_nbr(QuadTreeNode *node);
  QuadTreeNode* south_nbr(QuadTreeNode *node);
  QuadTreeNode* east_nbr(QuadTreeNode *node);
  QuadTreeNode* west_nbr(QuadTreeNode *node);

  std::queue<QuadTreeNode*> queue_leaves();
};

#endif // QUADTREENODE_H
