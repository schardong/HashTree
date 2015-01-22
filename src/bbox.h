#ifndef BBOX_H
#define BBOX_H

#include <glm/glm.hpp>
#include <array>

class BBox
{
public:
  BBox() {}
  virtual ~BBox() {}

  virtual bool PointInBox(glm::vec2 p) = 0;
  virtual bool Intersect(BBox& box) = 0;
};

////////////////////
class AABBox : public BBox
{
public:
  AABB(glm::vec2 bl = glm::vec2(0, 0), double edge = 1.0) :
    BBox(), bl_corner(bl), edge_sz(edge)
  {}

  bool PointInBox(glm::vec2 p)
  {
    bool x = (p.x >= bl_corner.x) && (p.x < bl_corner.x + edge_sz);
    bool y = (p.y >= bl_corner.y) && (p.y < bl_corner.y + edge_sz);
    return x && y;
  }

  bool Intersect(BBox& rhs)
  {
    return Intersect_p((AABB&)rhs);
  }

  double GetEdgeSz()
  {
    return edge_sz;
  }

  glm::vec2 GetCorner()
  {
    return bl_corner;
  }

  void SetEdgeSz(double e)
  {
    edge_sz = e;
  }

  void SetCorner(glm::vec2 c)
  {
    bl_corner = c;
  }

private:
  glm::vec2 bl_corner;
  double edge_sz;

  bool Intersect_p(AABB& rhs)
  {
    return PointInBox(rhs.bl_corner) || rhs.PointInBox(bl_corner);
  }
};

////////////////////
class Rhombus : public BBox
{
public:
  Rhombus()
  {
    m_corners[0] = glm::vec2(0, 0);
    m_corners[1] = glm::vec2(0, 0);
    m_corners[2] = glm::vec2(0, 0);
    m_corners[3] = glm::vec2(0, 0);
  }

  Rhombus(std::array<glm::vec2, 4>& c) :
    m_corners(c)
  {}

  bool PointInBox(glm::vec2 p)
  {
    using namespace glm;
    if(p == m_corners[0] || p == m_corners[1] || p == m_corners[2] || p == m_corners[3])
      return true;
    bool a = dot(normalize(m_corners[1] - m_corners[0]), normalize(p - m_corners[0])) >= 0;
    bool b = dot(normalize(m_corners[2] - m_corners[1]), normalize(p - m_corners[1])) >= 0;
    bool c = dot(normalize(m_corners[3] - m_corners[2]), normalize(p - m_corners[2])) >= 0;
    bool d = dot(normalize(m_corners[0] - m_corners[3]), normalize(p - m_corners[3])) >= 0;
    return (a == b == c == d);
  }

  bool Intersect(BBox& rhs)
  {
    return Intersect_p((Rhombus&)rhs) || rhs.Intersect((Rhombus&)*this);
  }

private:
  std::array<glm::vec2 , 4> m_corners;

  bool Intersect_p(Rhombus& rhs)
  {
    bool a = PointInBox(rhs.m_corners[0]);
    bool b = PointInBox(rhs.m_corners[1]);
    bool c = PointInBox(rhs.m_corners[2]);
    bool d = PointInBox(rhs.m_corners[3]);
    return a || b || c || d;
  }
};

#endif // BBOX_H