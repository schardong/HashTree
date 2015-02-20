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
               int max_depth = -1,
               NODE_TYPE nt = ROOT,
               std::vector<Vertex*> p = std::vector<Vertex*>(),
               glm::vec3 color = glm::vec3(1, 0, 0));

  virtual ~QuadTreeNode();
  virtual void Split();
  virtual int AddPoint(Vertex* p);
  virtual std::vector<Vertex*> GetPointsInRange(BBox* range);
  QuadTreeNode* FindNeighbor(NBR_DIR dir);
  void draw();

  virtual bool IsLeaf()
  {
    return !(m_children[0] || m_children[1] || m_children[2] || m_children[3]);
  }

  size_t GetNumPoints()
  {
    return m_points.size();
  }

  size_t GetMaxPoints()
  {
    return m_max_points;
  }

  int GetMaxDepth()
  {
    return m_max_depth;
  }

  Vertex* GetVertex(size_t i)
  {
    assert(i < m_points.size());
    return m_points[i];
  }

  size_t GetId()
  {
    return id;
  }

  BBox* GetBBox()
  {
    return m_box;
  }

  QuadTreeNode* GetChild(int idx)
  {
    assert(idx >= 0 && idx <= 3);
    return m_children[idx];
  }

  int GetDepth()
  {
    return m_depth;
  }

  void SetDepth(int d)
  {
    m_depth = d;
  }

  QuadTreeNode* GetParent()
  {
    return m_parent;
  }

  void SetParent(QuadTreeNode* p)
  {
    m_parent = p;
  }

  NODE_TYPE GetNodeType()
  {
    return m_node_type;
  }

  glm::vec3 GetColor()
  {
    return m_color;
  }

  void SetColor(glm::vec3 c)
  {
    m_color = c;
  }

private:
  size_t id;
  size_t m_max_points;
  int m_depth;
  BBox* m_box;
  NODE_TYPE m_node_type;
  int m_max_depth;
  QuadTreeNode* m_children[4];
  QuadTreeNode* m_parent;
  glm::vec3 m_color;
  std::vector<Vertex*> m_points;

  QuadTreeNode* north_nbr(QuadTreeNode *node);
  QuadTreeNode* south_nbr(QuadTreeNode *node);
  QuadTreeNode* east_nbr(QuadTreeNode *node);
  QuadTreeNode* west_nbr(QuadTreeNode *node);
};

#endif // QUADTREENODE_H
