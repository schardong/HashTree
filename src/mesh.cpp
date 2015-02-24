#include <iostream>
#include <vector>
#include <set>
#include <GL/glut.h>
#include "mesh.h"
#include "quadtree.h"
#include "quadtreenode.h"
#include "glm/glm.hpp"

using namespace std;
using namespace glm;

Mesh::Mesh(QuadTree* qt) :
  m_tree(qt)
{
  m_color = vec3(0, 1, 1);
}


Mesh::~Mesh()
{}

vector<QuadTreeNode*> queue_in_leaves(vector<QuadTreeNode*> leaves,
                                      vector<vec2> domain,
                                      int num_vertices_in)
{
  vector<QuadTreeNode*> in_leaves;

  if(leaves.empty() || domain.empty() || num_vertices_in <= 0 || num_vertices_in > 4)
    return in_leaves;

  size_t l_sz = leaves.size();

  for(size_t i = 0; i < l_sz; ++i) {
    BBox* bbox = leaves[i]->GetBBox();

    bool a = pnpoly(bbox->GetCorner(0), domain);
    bool b = pnpoly(bbox->GetCorner(1), domain);
    bool c = pnpoly(bbox->GetCorner(2), domain);
    bool d = pnpoly(bbox->GetCorner(3), domain);

    if((a + b + c + d) >= num_vertices_in)
      in_leaves.push_back(leaves[i]);
  }

  return in_leaves;
}

void Mesh::SetBaseMesh(vector<vec2*> domain)
{
  /*vector<QuadTreeNode*> leaves = m_tree->GetLeaves();
  vector<vec2> domain_vec(domain.size());

  size_t d_sz = domain.size();
  for(size_t i = 0; i < d_sz; ++i)
    domain_vec[i] = domain[i]->p;

  leaves = queue_in_leaves(leaves, domain_vec, 4);

  size_t l_sz = leaves.size();
  for(size_t i = 0; i < l_sz; ++i) {
    BBox* box = leaves[i]->GetBBox();
    m_faces.push_back(box);

    for(size_t j = 0; j < 4; ++j)
      m_vertices.push_back(new Vertex(box->GetCorner(j)));

    Vertex* v1 = m_vertices[m_vertices.size() - 4];
    Vertex* v2 = m_vertices[m_vertices.size() - 3];
    Vertex* v3 = m_vertices[m_vertices.size() - 2];
    Vertex* v4 = m_vertices[m_vertices.size() - 1];*/

    /*m_edges.push_back(new Edge(v1, v2));
    m_edges.push_back(new Edge(v2, v4));
    m_edges.push_back(new Edge(v4, v1));

    m_edges.push_back(new Edge(v2, v3));
    m_edges.push_back(new Edge(v3, v4));
    m_edges.push_back(new Edge(v4, v2));
  }*/
}

void Mesh::Triangulate()
{

}

void Mesh::draw()
{
  glColor3f(0, 1, 1);
  size_t v_sz = m_vertices.size();
  for(size_t i = 0; i < v_sz; ++i) {
    glBegin(GL_POINTS);
      glVertex2f(m_vertices[i]->x, m_vertices[i]->y);
    glEnd();
  }

  /*glColor3f(1, 0, 0);
  size_t e_sz = m_edges.size();
  for(size_t i = 0; i < e_sz; ++i) {
    Vertex* u = m_edges[i]->u;
    Vertex* v = m_edges[i]->v;
    glBegin(GL_LINES);
      glVertex2f(u->p.x, u->p.y);
      glVertex2f(v->p.x, v->p.y);
    glEnd();
  }*/
}