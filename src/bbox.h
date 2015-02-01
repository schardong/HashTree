#ifndef BBOX_H
#define BBOX_H

#include <glm/glm.hpp>
#include <array>


class BBox
{
public:
  BBox()
  {
    m_corners[0] = glm::vec2(0, 0);
    m_corners[1] = glm::vec2(0, 0);
    m_corners[2] = glm::vec2(0, 0);
    m_corners[3] = glm::vec2(0, 0);
  }

  BBox(std::array<glm::vec2, 4>& c) :
    m_corners(c)
  {}

  bool PointInBox(glm::vec2 p)
  {
    using namespace glm;
    if(p == m_corners[0] || p == m_corners[1] || p == m_corners[2] || p == m_corners[3])
      return true;

    for(int i = 1; i <= 4; ++i) {
      vec2 v1 = normalize(m_corners[i % 4] - m_corners[(i - 1) % 4]);
      vec2 v2 = normalize(p - m_corners[(i - 1) % 4]);

      if(v2.x * v1.y > v2.y * v1.x)
        return false;
    }

    return true;
  }

  bool Intersect(BBox& rhs)
  {
    return intersect(rhs) || rhs.intersect(*this);
  }

  glm::vec2 GetCorner(size_t i)
  {
    if(i > 3)
      return m_corners[0];
    return m_corners[i];
  }

private:
  std::array<glm::vec2 , 4> m_corners;

  bool intersect(BBox& rhs)
  {
    bool a = PointInBox(rhs.m_corners[0]);
    bool b = PointInBox(rhs.m_corners[1]);
    bool c = PointInBox(rhs.m_corners[2]);
    bool d = PointInBox(rhs.m_corners[3]);
    return a || b || c || d;
  }
};

#endif // BBOX_H
