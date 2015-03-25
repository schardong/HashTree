#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

class vertex
{
private:
  glm::vec2 m_coord;
  int m_id;

public:
  vertex(glm::vec2 p = glm::vec2(0));
  vertex(vertex& rhs);
  ~vertex();

  int GetId()
  {
    return m_id;
  }

  glm::vec2 GetCoord()
  {
    return m_coord;
  }

  void SetCoord(glm::vec2 p)
  {
    m_coord = p;
  }
};

#endif // VERTEX_H
