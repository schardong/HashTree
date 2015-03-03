#include "vertex.h"

static int g_curr_vertex_id = 0;

vertex::vertex(glm::vec2 p) :
  m_coord(p)
{
  m_id = g_curr_vertex_id++;
}

vertex::~vertex()
{
  m_id = -1;
  m_coord = glm::vec2(0);
}
