#ifndef BBOX_H
#define BBOX_H

#include <array>
#include <GL/glut.h>
#include <glm/glm.hpp>

#include "vertex.h"

class BBox
{
public:
  BBox()
  {}

  BBox(std::array<vertex, 4>& c) :
    m_corners(c)
  {}

  bool PointInBox(glm::vec2 p)
  {
    using namespace glm;
    if(p == m_corners[0].GetCoord() ||
       p == m_corners[1].GetCoord() ||
       p == m_corners[2].GetCoord() ||
       p == m_corners[3].GetCoord())
      return true;

    for(int i = 1; i <= 4; ++i) {
      vec2 v1 = normalize(m_corners[i % 4].GetCoord() - m_corners[(i - 1) % 4].GetCoord());
      vec2 v2 = normalize(p - m_corners[(i - 1) % 4].GetCoord());

      if(v2.x * v1.y > v2.y * v1.x)
        return false;
    }

    return true;
  }

  bool Intersect(BBox& rhs)
  {
    return intersect(rhs) || rhs.intersect(*this);
  }

  vertex GetCorner(size_t i)
  {
    if(i > 3)
      return m_corners[0];
    return m_corners[i];
  }

  void draw()
  {
    glBegin(GL_QUADS);
      glVertex2f(m_corners[0].GetCoord().x, m_corners[0].GetCoord().y);
      glVertex2f(m_corners[1].GetCoord().x, m_corners[1].GetCoord().y);
      glVertex2f(m_corners[2].GetCoord().x, m_corners[2].GetCoord().y);
      glVertex2f(m_corners[3].GetCoord().x, m_corners[3].GetCoord().y);
    glEnd();
  }

private:
  std::array<vertex, 4> m_corners;

  bool intersect(BBox& rhs)
  {
    bool a = PointInBox(rhs.m_corners[0].GetCoord());
    bool b = PointInBox(rhs.m_corners[1].GetCoord());
    bool c = PointInBox(rhs.m_corners[2].GetCoord());
    bool d = PointInBox(rhs.m_corners[3].GetCoord());
    return a || b || c || d;
  }
};

#endif // BBOX_H
