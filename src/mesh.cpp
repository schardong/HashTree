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

Mesh::Mesh(QuadTree* qt)
{
  m_color = vec3(0, 1, 1);
  BuildGeometry(qt);
}

Mesh::~Mesh()
{
  cleanup();
}

void Mesh::BuildGeometry(QuadTree* qt)
{
  if(qt == nullptr)
    return;

  if(m_vertices.size() > 0)
    cleanup();

  vector<QuadTreeNode*> leaves = qt->GetLeaves();

  build_geom_container(leaves);
  build_he_container(leaves);
  build_opp_container();
}

void Mesh::draw()
{
  glColor3f(m_color.r, m_color.g, m_color.b);

  size_t e_sz = m_edges.size();
  for(size_t i = 0; i < e_sz; ++i) {
    vertex u = m_vertices[m_edges[i]];
    vertex v = m_vertices[m_edges[next_edge(i)]];
    glBegin(GL_LINES);
      glVertex2f(u.GetCoord().x, u.GetCoord().y);
      glVertex2f(v.GetCoord().x, v.GetCoord().y);
    glEnd();
  }
}

void Mesh::build_geom_container(std::vector<QuadTreeNode*> leaves)
{
  if(leaves.empty())
    return;

  map<int, vertex> geom;

  size_t l_sz = leaves.size();
  for(size_t i = 0; i < l_sz; ++i) {
    BBox* box = leaves[i]->GetBBox();

    for(int j = 0; j < 4; ++j) {
      vertex v = box->GetCorner(j);
      int vid = v.GetId();
      if(geom.find(vid) == geom.end())
        geom[vid] = v;
    }
  }

  m_vertices.resize(geom.size());
  size_t i = 0;
  for(auto& it : geom) {
    m_vertices[i] = it.second;
    ++i;
  }
}

void Mesh::build_he_container(vector<QuadTreeNode*> leaves)
{
  if(m_vertices.empty())
    return;

  vector<int> tmp_edges;
  size_t l_sz = leaves.size();
  size_t v_sz = m_vertices.size();

  for(size_t i = 0; i < l_sz; ++i) {
    BBox* box = leaves[i]->GetBBox();
    for(int j = 0; j < 4; ++j) {
      vertex v = box->GetCorner(j);
      int vid = v.GetId();
      tmp_edges.push_back(vid);
    }
  }

  size_t idx = 0;
  size_t te_sz = tmp_edges.size();
  m_edges.resize(te_sz);
  for(size_t j = 0; j < te_sz; ++j) {
    for(idx = 0; idx < v_sz; ++idx) {
      if(tmp_edges[j] == m_vertices[idx].GetId())
        break;
    }
    m_edges[j] = idx;
  }
}

void Mesh::build_opp_container()
{
  if(m_edges.empty())
    return;

  map<pair<int, int>, int> adj_map;
  size_t e_sz = m_edges.size();

  for(size_t i = 0; i < e_sz; ++i) {
    pair<int, int> p(m_edges[i], m_edges[next_edge(i)]);
    adj_map[p] = i;
  }

  m_opposites.resize(e_sz, -1);

  for(auto& it : adj_map) {
    pair<int, int> p(it.first.second, it.first.first);

    auto it2 = adj_map.find(p);
    if(it2 != adj_map.end()) {
      m_opposites[it.second] = it2->second;
      m_opposites[it2->second] = it.second;
    }
  }

  adj_map.clear();
}