#ifndef QUADTREENODE_H
#define QUADTREENODE_H

#include <vector>
#include <queue>
#include <glm/glm.hpp>
#include "bbox.h"

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

enum PATTERN_TYPE
{
  PAT_CORNER_0,
  PAT_CORNER_1,
  PAT_CORNER_2,
  PAT_CORNER_3,
  NO_PAT
};

class QuadTreeNode
{
public:
  QuadTreeNode();
  QuadTreeNode(BBox* box,
               size_t max_npoints = 64,
               int max_depth = -1,
               NODE_TYPE nt = ROOT,
               std::vector<glm::vec2*> p = std::vector<glm::vec2*>(),
               glm::vec3 color = glm::vec3(1, 0, 0));

  virtual ~QuadTreeNode();
  virtual void Split(PATTERN_TYPE tp = NO_PAT);
  virtual int AddPoint(glm::vec2* p);
  virtual std::vector<glm::vec2*> GetPointsInRange(BBox* range);
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

  glm::vec2* GetVertex(size_t i)
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

  void SetChild(int idx, void* val)
  {
    assert(idx >= 0 && idx <= 3);
    m_children[idx] = (QuadTreeNode*)val;
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
  int m_max_depth;
  BBox* m_box;
  NODE_TYPE m_node_type;
  QuadTreeNode* m_children[4];
  QuadTreeNode* m_parent;
  std::vector<glm::vec2*> m_points;
  glm::vec3 m_color;
  bool m_degenerated;

  QuadTreeNode* north_nbr(QuadTreeNode *node);
  QuadTreeNode* south_nbr(QuadTreeNode *node);
  QuadTreeNode* east_nbr(QuadTreeNode *node);
  QuadTreeNode* west_nbr(QuadTreeNode *node);
};

#endif // QUADTREENODE_H
