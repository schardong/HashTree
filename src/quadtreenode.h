#ifndef QUADTREENODE_H
#define QUADTREENODE_H

#include <vector>
#include <glm/glm.hpp>

struct AABB
{
  glm::vec2 bl_corner;
  double edge_sz;

  AABB(glm::vec2 bl = glm::vec2(0, 0), double edge = 1.0) :
    bl_corner(bl), edge_sz(edge)
  {}

  bool PointInBox(glm::vec2 p)
  {
    bool x = (p.x >= bl_corner.x) && (p.x < bl_corner.x + edge_sz);
    bool y = (p.y >= bl_corner.y) && (p.y < bl_corner.y + edge_sz);
    return x && y;
  }

  bool Intersect(AABB& rhs)
  {
    return PointInBox(rhs.bl_corner) || rhs.PointInBox(bl_corner);
//    glm::vec2 c = bl_corner - rhs.bl_corner;
//    double l = (edge_sz + rhs.edge_sz) * (edge_sz + rhs.edge_sz) / 4.f;

//    if(glm::dot(c, c) <= l) return true;
//    return false;
  }
};

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
