#ifndef QUADTREENODE_H
#define QUADTREENODE_H

#include "bbox.h"
#include <vector>
#include <glm/glm.hpp>

//struct AABB
//{
//  glm::vec2 bl_corner;
//  double edge_sz;
//
//  AABB(glm::vec2 bl = glm::vec2(0, 0), double edge = 1.0) :
//    bl_corner(bl), edge_sz(edge)
//  {}
//
//  bool PointInBox(glm::vec2 p)
//  {
//    bool x = (p.x >= bl_corner.x) && (p.x < bl_corner.x + edge_sz);
//    bool y = (p.y >= bl_corner.y) && (p.y < bl_corner.y + edge_sz);
//    return x && y;
//  }
//
//  bool Intersect(AABB& rhs)
//  {
//    return PointInBox(rhs.bl_corner) || rhs.PointInBox(bl_corner);
//  }
//};
//
//struct Rhombus
//{
//  glm::vec2 m_corners[4];
//
//  bool PointInRhombus(glm::vec2 p)
//  {
//    using namespace glm;
//    bool a = dot(m_corners[1] - m_corners[0], p) > 0;
//    bool b = dot(m_corners[2] - m_corners[1], p) > 0;
//    bool c = dot(m_corners[3] - m_corners[2], p) > 0;
//    bool d = dot(m_corners[0] - m_corners[3], p) > 0;
//    return a && b && c && d;
//  }
//};

class QuadTreeNode
{
public:
  QuadTreeNode(size_t max_npoints = 64,
               AABB* box = new AABB,
               std::vector<glm::vec2> p = std::vector<glm::vec2>());

  virtual ~QuadTreeNode();

  virtual bool IsLeaf()
  {
    return (!children[0] && !children[1] && !children[2] && !children[3]);
  }

  virtual void Split();
  virtual int AddPoint(glm::vec2 p);
  virtual std::vector<glm::vec2> GetPointsInRange(AABB* range);

  size_t GetNumPoints()
  {
    return points.size();
  }

  AABB* GetBBox()
  {
    return bbox;
  }

  size_t GetDepth()
  {
    return depth;
  }

  void SetDepth(size_t d)
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

private:
  size_t id;
  size_t depth;
  size_t max_points;
  AABB* bbox;
  QuadTreeNode* children[4];
  QuadTreeNode* parent;
  std::vector<glm::vec2> points;
};

#endif // QUADTREENODE_H
