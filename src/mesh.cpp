#include <iostream>
#include <vector>
#include <map>
#include <GL/glut.h>
#include <glm/glm.hpp>

#include "mesh.h"
#include "vertex.h"
#include "bbox.h"
#include "quadtree.h"
#include "quadtreenode.h"

using namespace std;
using namespace glm;

Mesh::Mesh(QuadTree* qt) :
  m_tree(qt)
{
  assert(qt != nullptr);
  m_color = vec3(0, 1, 1);

  map<int, vertex> shared_verts;
  vector<int> tmp_edges;
  vector<QuadTreeNode*> leaves = qt->GetLeaves();
  
  size_t i;
  size_t l_sz = leaves.size();
  for(i = 0; i < l_sz; ++i) {
    BBox* box = leaves[i]->GetBBox();

    for(int j = 0; j < 4; ++j) {
      vertex v = box->GetCorner(j);
      int vid = v.GetId();
      if(shared_verts.find(vid) == shared_verts.end())
        shared_verts[vid] = v;

      tmp_edges.push_back(vid);
    }
  }

  m_vertices.resize(shared_verts.size());
  i = 0;
  for(auto it = shared_verts.begin(); it != shared_verts.end(); ++it, ++i)
    m_vertices[i] = it->second;

  size_t v_sz = m_vertices.size();
  size_t te_sz = tmp_edges.size();
  m_edges.resize(te_sz);
  for(size_t j = 0; j < te_sz; ++j) {
    for(i = 0; i < v_sz; ++i) {
      if(tmp_edges[j] == m_vertices[i].GetId())
        break;
    }
    m_edges[j] = i;
  }

  printf("number of vertices: %d\n", shared_verts.size());
}

Mesh::~Mesh()
{}

void Mesh::draw()
{
  glColor3f(0, 1, 1);
  /*size_t v_sz = m_vertices.size();
  for(size_t i = 0; i < v_sz; ++i) {
    glBegin(GL_POINTS);
    glVertex2f(m_vertices[i].GetCoord().x, m_vertices[i].GetCoord().y);
    glEnd();
  }*/


  size_t e_sz = m_edges.size();
  for(size_t i = 0; i < e_sz; ++i) {
    vertex u = m_vertices[m_edges[i]];
    vertex v = m_vertices[m_edges[next_edge(i)]];
    glBegin(GL_LINES);
      glVertex2f(u.GetCoord().x, u.GetCoord().y);
      glVertex2f(v.GetCoord().x, v.GetCoord().y);
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
