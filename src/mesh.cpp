#include <iostream>
#include <vector>
#include <set>
#include <gl/glut.h>
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

void Mesh::SetBaseMesh(vector<Vertex*> domain)
{
  vector<QuadTreeNode*> leaves = m_tree->GetLeaves();
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
    for(size_t j = 1; j <= 4; ++j) {
      size_t idx1 = (j - 1) % 4;
      size_t idx2 = j % 4;
    }
  }
}

void Mesh::Triangulate()
{

}

void Mesh::draw()
{
  size_t f_sz = m_faces.size();
  for(size_t i = 0; i < f_sz; ++i) 
    m_faces[i]->draw();
}

//bool vec_comp(Vertex a, Vertex b)
//{
//  return a.p.x < b.p.x;
//}
//
//void SetVertices(QuadTree* qt)
//{
//  if(!qt) return;
//
//  vector<QuadTreeNode*> leaves = qt->GetLeaves();
//  size_t l_sz = leaves.size();
//
//  set<Vertex, bool(*)(Vertex, Vertex)> v_set(vec_comp);
//  for(size_t i = 0; i < l_sz; ++i) {
//    BBox* box = leaves[i]->GetBBox();
//
//    for(int j = 0; j < 4; ++j) {
//      Edge e;
//      Vertex a(box->GetCorner(j));
//    }
//
//    v_set.insert(box->GetCorner(0));
//    v_set.insert(box->GetCorner(1));
//    v_set.insert(box->GetCorner(2));
//    v_set.insert(box->GetCorner(3));
//  }
//
//  cout << v_set.size() << endl;
//}
//
//void InclusionTest(std::vector<glm::vec2> domain)
//{
//  if(m_vertices.empty() || domain.empty())
//    return;
//
//  vector<Vertex> in_vertices;
//
//  size_t v_sz = m_vertices.size();
//  for(size_t i = 0; i < v_sz; ++i)
//    if(pnpoly(m_vertices[i].p, domain))
//      in_vertices.push_back(m_vertices[i]);
//
//  m_vertices.clear();
//  m_vertices = in_vertices;
//}