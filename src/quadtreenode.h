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

  bool Intersect(AABB& rhs)
  {
    glm::vec2 c = bl_corner - rhs.bl_corner;
    size_t l = (edge_sz + rhs.edge_sz) * (edge_sz + rhs.edge_sz) / 4;

    if(glm::dot(c, c) <= l) return true;
    return false;
  }

  bool PointInBox(glm::vec2 p)
  {
    bool x = (p.x >= bl_corner.x) && (p.x < bl_corner.x + edge_sz);
    bool y = (p.y >= bl_corner.y) && (p.y < bl_corner.y + edge_sz);
    return x && y;
  }

};

class QuadTreeNode
{
public:
  QuadTreeNode(size_t max_npoints = 64,
               AABB* box = new AABB(),
               std::vector<glm::vec2> p = std::vector<glm::vec2>());

  virtual ~QuadTreeNode();

  virtual bool IsLeaf()
  {
    return (!children[0] && !children[1] && !children[2] && !children[3]);
  }

  virtual void Split();
  virtual void AddPoint(glm::vec2 p);

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

private:
  size_t id;
  size_t depth;
  size_t max_points;
  AABB* bbox;
  QuadTreeNode* children[4];
  std::vector<glm::vec2> points;
};

#endif // QUADTREENODE_H
