#ifndef QUADTREENODE_H
#define QUADTREENODE_H

#include "bbox.h"
#include "edge.h"
#include <vector>
#include <queue>
#include <glm/glm.hpp>

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
               size_t max_npoints = 64,
               NODE_TYPE nt = ROOT,
               glm::vec3 color = glm::vec3(1, 0, 0),
               std::vector<Vertex*> p = std::vector<Vertex*>());

  virtual ~QuadTreeNode();

  virtual bool IsLeaf()
  {
    return (!children[0] && !children[1] && !children[2] && !children[3]);
  }

  virtual void Split();
  virtual int AddPoint(Vertex* p);
  virtual std::vector<Vertex*> GetPointsInRange(BBox* range);

  size_t GetNumPoints()
  {
    return points.size();
  }

  Vertex* GetVertex(size_t i)
  {
    assert(i < points.size());
    return points[i];
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

  glm::vec3 GetColor()
  {
    return m_color;
  }

  void SetColor(glm::vec3 c)
  {
    m_color = c;
  }

  QuadTreeNode* FindNeighbor(NBR_DIR dir);
  void draw();
  void delEmptyLeaves();

private:
  size_t id;
  size_t max_points;
  int depth;
  BBox* bbox;
  NODE_TYPE node_type;
  QuadTreeNode* children[4];
  QuadTreeNode* parent;
  glm::vec3 m_color;
  std::vector<Vertex*> points;

  QuadTreeNode* north_nbr(QuadTreeNode *node);
  QuadTreeNode* south_nbr(QuadTreeNode *node);
  QuadTreeNode* east_nbr(QuadTreeNode *node);
  QuadTreeNode* west_nbr(QuadTreeNode *node);
};

#endif // QUADTREENODE_H
